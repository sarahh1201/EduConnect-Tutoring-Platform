#pragma once
#include <string>
#include <vector>
#include "RequestHeap.h"

using std::string;

// ----------------- User Struct -----------------
struct User
{
    string username, firstName, lastName, password, email, securityQ, securityA;
    int userType; // 1 = Tutor, 0 = Student
};

// ----------------- Tutor Struct -----------------
struct Tutor
{
    int tutorID;
    string name;
    string username;
    vector<string> subjects;
    string email;
    double rating;
    bool available;
};

// ----------------- Student Request Struct -----------------
struct Request
{
    int requestID;
    string studentUsername;
    string subject;
    int urgency;
    string description;
};

struct Rating
{
    int ratingID;
    string tutorUsername;
    string studentUsername;
    int ratingValue;
    string comments;
};

struct Session
{
    int sessionID;
    string subject;
    string description;
    string tutorUsername;
    string studentUsername;
    int requestID;
    string status;
};

struct SessionNode {
    Session data;       // the actual session info
    SessionNode* next;  // pointer to next node

    SessionNode(Session s) : data(s), next(nullptr) {}
};

struct SessionHistory {
    SessionNode* head = nullptr;

    void addSession(Session s) {
        SessionNode* newNode = new SessionNode(s);
        newNode->next = head;
        head = newNode;
    }

    string display() const {
        SessionNode* current = head;
        string session;
        session+="Session History <br><hr>";
        while (current) {
            session += "Session ID: " + to_string(current->data.sessionID) + "<br>";
            session += "Subject: " + current->data.subject + "<br>";
            session += "Description: " + current->data.description + "<br>";
            session += "Tutor Username: " + current->data.tutorUsername + "<br>";
            session += "Student Username: " + current->data.studentUsername + "<br>";
            session += "Request ID: " + to_string(current->data.requestID) + "<br>";
            session += "Status: " + current->data.status + "<br><hr>";
            current = current->next;
        }
        return session;
    }
};

// CSV file paths as extern
extern string userDataPath;
extern string tutorsDataPath;
extern string requestsDataPath;
extern string sessionsDataPath;
extern string subjectsDataPath;
extern string ratingsDataPath;

// Function declarations
std::vector<User> loadUsersCSV();
void saveUsersCSV(const std::vector<User>& users);

std::vector<Tutor> loadTutorsCSV();
void saveTutorsCSV(const vector<Tutor> &tutors);

std::vector<Request> loadRequestsCSV();
void saveRequestsCSV(const std::vector<Request>& requests);
void removeRequestFromCSV(int requestID);
MinHeap* buildRequestHeapFromCSV();

std::vector<Rating> loadRatingsCSV();
void saveRatingsCSV(const std::vector<Rating>& ratings);

SessionHistory loadSessionsCSV();
void saveSessionsCSV(const SessionHistory &history);

const std::vector<std::string> loadSubjectsCSV();
void SubjectsList();


