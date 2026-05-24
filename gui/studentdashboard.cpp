#include "studentdashboard.h"
#include "ui_studentdashboard.h"
#include "loginwindow.h"
#include "backend/Decode.h"
#include "backend/Backend.h"
#include <QMessageBox>
#include <string>
#include <fstream>

studentdashboard::studentdashboard(const string& username, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::studentdashboard)
    , username(username)
{
    ui->setupUi(this);

    // Display the logged-in tutor's name
    QString userName = QString::fromStdString(username);
    QString htmlTemplate = QString(
        "<p style=\"font-size:36pt; font-weight:bold; color:#354680;\">"
        "Welcome, %1!</p>"
        );

    ui->toptext->setHtml(htmlTemplate.arg(userName));


    ui->searchoptions->addItem("By Tutor's ID");
    ui->searchoptions->addItem("By Tutor's Name");
    ui->searchoptions->addItem("By Subject");
    ui->searchoptions->addItem("By Rating Threshold");

    ViewSessions(username);

    vector<std::string> subjects = loadSubjectsCSV();

    for (const auto &s : subjects) {
        ui->subjects->addItem(QString::fromStdString(s));
    }


    // Example: You can load tutor info here if need
}

studentdashboard::~studentdashboard()
{
    delete ui;
}

void studentdashboard::on_logout_btn_clicked()
{
    loginwindow *dashboard = new loginwindow();
    dashboard->show();
    this->hide();
}

void studentdashboard::on_refresh_btn_clicked()
{
    ViewSessions(username);
}

void studentdashboard::on_searchtutor_btn_clicked()
{
    QString option = ui->searchoptions->currentText();
    QString input = ui->search_in->text();
    vector<Tutor> tutors = loadTutorsCSV();
    QString results;

    if (option == "By Tutor's ID") {
        int id = input.toInt();
        bool found = false;
        for (const Tutor &t : tutors) {
            if (t.tutorID == id) {
                results += QString("Tutor #%1<br>").arg(t.tutorID);
                results += QString("Name: %1<br>").arg(QString::fromStdString(t.name));
                results += QString("Subjects: %1<br>").arg(joinSubjects(t.subjects));
                results += QString("Rating: %1<br>").arg(t.rating);
                results += QString("Email: %1<hr>").arg(QString::fromStdString(t.email));
                found = true;
                break;
            }
        }
        if (!found) results = "No tutor found with that ID.";
    }
    else if (option == "By Tutor's Name") {
        bool found = false;
        for (const Tutor &t : tutors) {
            if (QString::fromStdString(t.name).contains(input, Qt::CaseInsensitive)) {
                results += QString("Tutor #%1<br>").arg(t.tutorID);
                results += QString("Name: %1<br>").arg(QString::fromStdString(t.name));
                results += QString("Subjects: %1<br>").arg(joinSubjects(t.subjects));
                results += QString("Rating: %1<br>").arg(t.rating);
                results += QString("Email: %1<hr>").arg(QString::fromStdString(t.email));
                found = true;
            }
        }
        if (!found) results = "No tutors found with that name.";
    }
    else if (option == "By Subject") {
        bool found = false;
        for (const Tutor &t : tutors) {
            if (std::find(t.subjects.begin(), t.subjects.end(), input.toStdString()) != t.subjects.end()) {
                results += QString("Tutor #%1<br>").arg(t.tutorID);
                results += QString("Name: %1<br>").arg(QString::fromStdString(t.name));
                results += QString("Subjects: %1<br>").arg(joinSubjects(t.subjects));
                results += QString("Rating: %1<br>").arg(t.rating);
                results += QString("Email: %1<hr>").arg(QString::fromStdString(t.email));
                found = true;
            }
        }
        if (!found) results = "No tutors found for that subject.";
    }
    else if (option == "By Rating Threshold") {
        bool found = false;
        double threshold = input.toDouble();
        for (const Tutor &t : tutors) {
            if (t.rating >= threshold) {
                results += QString("Tutor #%1<br>").arg(t.tutorID);
                results += QString("Name: %1<br>").arg(QString::fromStdString(t.name));
                results += QString("Subjects: %1<br>").arg(joinSubjects(t.subjects));
                results += QString("Rating: %1<br>").arg(t.rating);
                results += QString("Email: %1<hr>").arg(QString::fromStdString(t.email));
                found = true;
            }
        }
        if (!found) results = "No tutors found above that rating.";
    }

    ui->search_out->setHtml(results);
}

// Helper function to join vector<string> into one comma-separated string
QString studentdashboard::joinSubjects(const vector<string>& subjects) {
    QStringList list;
    for (const string &s : subjects) list << QString::fromStdString(s);
    return list.join(", ");
}


void studentdashboard::on_createreq_btn_clicked()
{
    Request newRequest;
    newRequest.studentUsername = username;
    newRequest.subject = ui->subjects->currentText().toStdString();
    newRequest.urgency = ui->urgency->value();
    newRequest.description = ui->desc->text().toStdString();

    if (ui->subjects->currentText().isEmpty() ||
        ui->urgency->value() == 0 ||
        ui->desc->text().isEmpty())
    {
        QMessageBox::warning(this, "Invalid Input", "Please fill in all fields before submitting.");
        return;
    }

    vector<Request> requests = loadRequestsCSV();
    newRequest.requestID = requests.empty() ? 1 : requests.back().requestID + 1;

    ofstream file(requestsDataPath, ios::app);
    if (!file.is_open()) {
        ui->req_status->setText("Error: Could not save request.");
        return;
    }

    file << newRequest.requestID << ","
         << newRequest.studentUsername << ","
         << newRequest.subject << ","
         << newRequest.urgency << ","
         << newRequest.description << "\n";

    file.close();
    ui->req_status->setText("Request submitted successfully!");
}

void studentdashboard::ViewSessions(const string &studentUsername) {
    SessionHistory sessions = loadSessionsCSV();
    QString sessionInfo;
    QString rateTutors;
    SessionNode* current = sessions.head;
    bool hasActive = false;
    bool hasTutortoRate = false;

    while (current) {
        const Session &s = current->data;
        if (s.studentUsername == studentUsername && s.status == "accepted") {
            sessionInfo += "<b>Session ID:</b> " + QString::number(s.sessionID) + "<br>";
            sessionInfo += "<b>Subject:</b> " + QString::fromStdString(s.subject) + "<br>";
            sessionInfo += "<b>Description:</b> " + QString::fromStdString(s.description) + "<br>";
            sessionInfo += "<b>ID - Tutor Username:</b> " + QString::fromStdString(s.tutorUsername) + "<br>";
            sessionInfo += "<b>Status:</b> " + QString::fromStdString(s.status) + "<hr>";
            hasActive = true;
        }
        if (s.studentUsername == studentUsername && s.status == "ended") {
            rateTutors += "<b>Session ID:</b> " + QString::number(s.sessionID) + "<br>";
            rateTutors += "<b>Subject:</b> " + QString::fromStdString(s.subject) + "<br>";
            rateTutors += "<b>Description:</b> " + QString::fromStdString(s.description) + "<br>";
            rateTutors += "<b>ID - Tutor Username:</b> " + QString::fromStdString(s.tutorUsername) + "<br>";
            rateTutors += "<b>Status:</b> " + QString::fromStdString(s.status) + "<hr>";
            hasTutortoRate = true;
        }
        current = current->next;
    }

    if (!hasActive) {
        sessionInfo += "<i>You have no active sessions.</i><hr>";
    }
    if (!hasTutortoRate){
        rateTutors += "<i>There are no tutors to rate.</i><hr>";
    }
    ui->sessions_out->setHtml(sessionInfo+rateTutors);


}

void studentdashboard::on_endsession_btn_clicked()
{
    SessionHistory sessions = loadSessionsCSV();
    int sessionID = ui->session_id->text().toInt();
    bool sessionFound = false;

    // Traverse the linked list
    SessionNode* current = sessions.head;
    while (current) {
        Session &s = current->data;
        if (s.sessionID == sessionID && s.tutorUsername == username && s.status == "accepted") {
            s.status = "ended";
            sessionFound = true;
            break;
        }
        current = current->next;
    }

    if (sessionFound) {
        saveSessionsCSV(sessions);
        ui->sessions_out->setText("Session " + QString::number(sessionID) + " marked as completed.");
    } else {
        ui->sessions_out->setText("Session not found or cannot be ended.");
    }
}

void studentdashboard::on_ratetutor_btn_clicked()
{
    QString tutor = ui->tutor_user->text().trimmed(); // Tutor username
    int rating = ui->rating_in->value();               // Rating value (1-5)
    QString comment = ui->comments->text().trimmed(); // Comment

    if (tutor.isEmpty() || rating == 0 || comment.isEmpty()) {
        ui->sessions_out->setText("Please fill all fields before submitting.");
        return;
    }

    // Load session history
    SessionHistory history = loadSessionsCSV();
    SessionNode* current = history.head;
    bool sessionFound = false;

    // Find a session that matches tutor, current user, and is ended
    while (current) {
        if (current->data.tutorUsername == tutor.toStdString() &&
            current->data.studentUsername == username &&
            current->data.status == "ended") {
            sessionFound = true;
            break;
        }
        current = current->next;
    }

    if (!sessionFound) {
        ui->sessions_out->setText("Session not found or cannot be rated.");
        return;
    }

    // Append rating to ratings CSV
    vector<Rating> ratings = loadRatingsCSV();
    int nextRatingID = ratings.empty() ? 1 : ratings.back().ratingID + 1;

    ofstream file(ratingsDataPath, ios::app);
    if (!file.is_open()) {
        ui->sessions_out->setText("Failed to open ratings CSV!");
        return;
    }
    file << nextRatingID << ","
         << tutor.toStdString() << ","
         << username << ","
         << rating << ","
         << comment.toStdString() << "\n";
    file.close();

    // Update tutor's average
    updateTutorAverage(tutor.toStdString());

    // Update session status to "reviewed"
    current->data.status = "reviewed";
    saveSessionsCSV(history);

    ui->sessions_out->setText("Rating submitted successfully and session marked as reviewed.");

    // Optional: clear input fields
    ui->tutor_user->clear();
    ui->rating_in->setValue(0);
    ui->comments->clear();
}

    

