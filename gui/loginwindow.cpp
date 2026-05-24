#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "studentdashboard.h"
#include "tutordashboard.h"
#include "admindashboard.h"
#include "backend/Decode.h"
#include "backend/Backend.h"
#include <QMessageBox>
#include <string>
#include <QDir>
loginwindow::loginwindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::loginwindow)
{
    ui->setupUi(this);
}

loginwindow::~loginwindow()
{
    delete ui;
}

void loginwindow::on_login_btn_clicked()
{
    auto users = loadUsersCSV();
    qDebug() << "Loaded users:" << users.size();


    qDebug() << "Current working directory:" << QDir::currentPath();
    QString usernameQt = ui->usr_in->text();
    QString passwordQt = ui->pswd_in->text();

    string username = usernameQt.toStdString();
    string password = passwordQt.toStdString();

    bool isTutor = false;
    if(username == "admin" && password == "admin"){
        admindashboard *dashboard = new admindashboard();
        dashboard->show();
        this->hide();
    }
    else {

    // This now calls the boolean password-verification function
    if (passwordCheck(username, password, isTutor)) {

        QMessageBox::information(this, "Success", "Login successful!");

        if (isTutor) {
            tutordashboard *dashboard = new tutordashboard(username, this);
            dashboard->show();
        } else {
            studentdashboard *dashboard = new studentdashboard(username, this);
            dashboard->show();
        }

        this->hide();
        return;
    }

    QMessageBox::warning(this, "Error", "Invalid username or password!");
    }
}

