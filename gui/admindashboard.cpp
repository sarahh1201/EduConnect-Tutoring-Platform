#include "admindashboard.h"
#include "ui_admindashboard.h"
#include "loginwindow.h"
#include "backend/Decode.h"
#include "backend/Backend.h"
#include "backend/RequestProcess.h"

admindashboard::admindashboard(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::admindashboard)
{
    ui->setupUi(this);
    ViewTutors();
    ViewStudents();
    ViewRequests();
    ViewSessions();
}

admindashboard::~admindashboard()
{
    delete ui;
}

void admindashboard::on_logout_btn_clicked()
{
    loginwindow *dashboard = new loginwindow();
    dashboard->show();
    this->hide();
}

void admindashboard::on_refresh_btn_clicked()
{
    ViewTutors();
    ViewStudents();
    ViewRequests();
    ViewSessions();
}
void admindashboard::ViewSessions()
{
    QString dispSessions;
    SessionHistory sessions = loadSessionsCSV();
    dispSessions = QString::fromStdString(sessions.display());
    ui->sessions->setHtml(dispSessions);
}

void admindashboard::ViewRequests()
{
    QString dispRequest;
    vector<Request> requests = loadRequestsCSV();
    vector<Tutor> tutors = loadTutorsCSV();
    dispRequest = QString::fromStdString(processRequests(requests, tutors));
    ui->requests->setHtml(dispRequest);
}

void admindashboard::ViewTutors()
{
    QString tot_tutors;
    int count = 0;
    vector<Tutor> tutors = loadTutorsCSV();

    quickSort(tutors, 0, tutors.size()-1);
    tot_tutors += "<hr>Top Tutors<hr>";

    for (size_t i=0; i < min(tutors.size(), size_t(3));i++)
    {
        const auto &t = tutors[tutors.size()-1-i];
        tot_tutors += QString::number(i+1);
        tot_tutors += " Name: "+ QString::fromStdString(t.name);
        tot_tutors += ", Username:"+QString::fromStdString(t.username);
        tot_tutors += ", Rating:"+QString::number(t.rating)+ "<br>";
    }

    tot_tutors += "<hr>All Tutors<hr>";
    for (const auto &t : tutors)
    {
        tot_tutors += "Name: "+ QString::fromStdString(t.name);
        tot_tutors += ", Username:"+QString::fromStdString(t.username);
        tot_tutors += ", Rating:"+QString::number(t.rating)+ "<br>";
        count++;
    }

    tot_tutors += "Total Tutors: " + QString::number(count);

    ui->tutors->setHtml(tot_tutors);
}

void admindashboard::ViewStudents()
{
    QString tot_students;
    int count = 0;
    vector<User> users = loadUsersCSV();



    tot_students += "<hr>All Students <hr>";
    for (const auto &u : users)
    {
        if (u.userType == 0)
        {
            tot_students += "Name: "+ QString::fromStdString(u.firstName) + QString::fromStdString(" ") + QString::fromStdString(u.lastName);
            tot_students += ", Username:"+QString::fromStdString(u.username)+"<br>";
            count++;
        }
    }

    tot_students += "Total Students: " + QString::number(count);

    ui->students->setHtml(tot_students);
}

