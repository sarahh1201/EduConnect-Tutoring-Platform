#include "tutordashboard.h"
#include "ui_tutordashboard.h"
#include "loginwindow.h"
#include "backend/Decode.h"

tutordashboard::tutordashboard(const string& username, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::tutordashboard)
    , username(username)
{
    ui->setupUi(this);

    // Display the logged-in tutor's name
    QString userName = QString::fromStdString(username);
    QString welcome = QString(
        "<p style=\"font-size:36pt; font-weight:bold; color:#354680;\">"
        "Welcome, %1!</p>"
        );
    ui->top->setHtml(welcome.arg(userName));

    ViewRequests(username);
    ViewRatings(username);
    ViewSessions(username);
}

tutordashboard::~tutordashboard()
{
    delete ui;
}

void tutordashboard::on_refresh_btn_clicked()
{
    ViewRequests(username);
    ViewRatings(username);
    ViewSessions(username);
}

void tutordashboard::on_logout_btn_clicked()
{
    loginwindow *dashboard = new loginwindow();
    dashboard->show();
    this->hide();
}

void tutordashboard::ViewRequests(const string &tutorUsername)
{
    vector<Request> requests = loadRequestsCSV();
    vector<Tutor> tutors = loadTutorsCSV();

    Tutor currentTutor;
    bool found = false;

    for (const auto &t : tutors)
    {
        if (t.username == tutorUsername)
        {
            currentTutor = t;
            found = true;
            break;
        }
    }

    if (!found) {
        ui->request_out->setText("No requests available.");
        return;
    }

    else{
    vector<Request> available;

    for (const auto &req : requests)
        if (tutorTeaches(currentTutor, req.subject))
            available.push_back(req);

    QString request;

    for (const auto &req : available)
    {
        request += "<b>Request ID:</b> " + QString::number(req.requestID) + "<br>";
        request += "<b>Student:</b> " + QString::fromStdString(req.studentUsername) + "<br>";
        request += "<b>Subject:</b> " + QString::fromStdString(req.subject) + "<br>";
        request += "<b>Description:</b> " + QString::fromStdString(req.description) + "<hr>";
    }

    ui->request_out->setHtml(request);
    }
}

bool tutordashboard::tutorTeaches(const Tutor &t, const string &subject)
{
    for (const string &s : t.subjects)
    {
        if (s == subject)
            return true;
    }
    return false;
}

void tutordashboard::AcceptRequest(const string &tutorUsername)
{
    vector<Request> requests = loadRequestsCSV();
    vector<Tutor> tutors = loadTutorsCSV();

    Tutor currentTutor;
    bool found = false;

    for (const auto &t : tutors)
    {
        if (t.username == tutorUsername)
        {
            currentTutor = t;
            found = true;
            break;
        }
    }

    if (!found)
    {
        ui->request_out->setText("No requests available.");
        return;
    }

    // Here you would normally get the request ID from user input in the UI
    // For demonstration, we'll assume a request ID is provided
    int reqID = ui->req_in->text().toInt();

    for (const auto &r : requests)
    {
        if (r.requestID == reqID)
        {
            // Ensure tutor teaches the requested subject
            if (!tutorTeaches(currentTutor, r.subject))
            {
                ui->request_out->setText("You cannot accept this request — you do NOT teach this subject.");
                return;
            }

            // Accept request
            ui->request_out->setText("Request " + QString::number(reqID) + " accepted successfully.");
            UpdateRequestStatus(reqID, tutorUsername);  // remove request + add session history
            return;
        }
    }

    ui->request_out->setText("Request ID not found.");
}

void tutordashboard::UpdateRequestStatus(int reqID, const string &tutorUsername)
{
    vector<Request> requests = loadRequestsCSV();

    // Find the request
    auto it = find_if(requests.begin(), requests.end(),
                      [reqID](const Request &r)
                      { return r.requestID == reqID; });
    if (it == requests.end())
    {
        ui->request_out->setText("Request ID not found.");
        return;
    }
    Request acceptedRequest = *it;

    // Remove request from CSV
    removeRequestFromCSV(reqID);

    // Load existing sessions, add new session
    SessionHistory history = loadSessionsCSV();

    int nextSessionID = 1; // default if no sessions exist
    SessionNode* current = history.head;
    while (current) {
        if (current->data.sessionID >= nextSessionID)
            nextSessionID = current->data.sessionID + 1;
        current = current->next;
    }


    Session newSession{
        nextSessionID,   // sessionID
        acceptedRequest.subject,     // subject
        acceptedRequest.description, // description
        tutorUsername,                          
        acceptedRequest.studentUsername,                       
        acceptedRequest.requestID,   // requestID
        "accepted"                   // status
    };
    history.addSession(newSession);

    // Save updated sessions
    saveSessionsCSV(history);

    ui->request_out->setText("Request " + QString::number(reqID) + " accepted and session created.");
}
void tutordashboard::on_acceptreq_btn_clicked()
{
    AcceptRequest(username);
}

void tutordashboard::ViewSessions(const string &tutorUsername) {
    SessionHistory sessions = loadSessionsCSV();
    QString sessionInfo;
    SessionNode* current = sessions.head;
    bool hasActive = false;

    while (current) {
        const Session &s = current->data;
        if (s.tutorUsername == tutorUsername && s.status == "accepted") {
            sessionInfo += "<b>Session ID:</b> " + QString::number(s.sessionID) + "<br>";
            sessionInfo += "<b>Subject:</b> " + QString::fromStdString(s.subject) + "<br>";
            sessionInfo += "<b>Description:</b> " + QString::fromStdString(s.description) + "<br>";
            sessionInfo += "<b>Student Username:</b> " + QString::fromStdString(s.studentUsername) + "<br>";
            sessionInfo += "<b>Status:</b> " + QString::fromStdString(s.status) + "<hr>";
            hasActive = true;
        }
        current = current->next;
    }

    if (!hasActive) {
        sessionInfo = "<i>You have no active sessions.</i>";
    }

    ui->sessions_out->setHtml(sessionInfo);
}

void tutordashboard::on_endsession_btn_clicked()
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


void tutordashboard::ViewRatings(const string &tutorUsername){
    vector<Rating> ratings = loadRatingsCSV();
    QString ratingInfo;
    for (const auto &r : ratings) {
        if (r.tutorUsername == tutorUsername) {
            ratingInfo += "<b>Rating ID:</b> " + QString::number(r.ratingID) + "<br>";
            ratingInfo += "<b>Student Username:</b> " + QString::fromStdString(r.studentUsername) + "<br>";
            ratingInfo += "<b>Rating Value:</b> " + QString::number(r.ratingValue) + "<br>";
            ratingInfo += "<b>Comments:</b> " + QString::fromStdString(r.comments) + "<hr>";
        }
    }
    ui->ratings_out->setHtml(ratingInfo);
}
