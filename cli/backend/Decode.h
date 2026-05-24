#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <string>
#include "SessionHistory.h"
#include "BinarySearchTree.h"
#include "RequestHeap.h"
using namespace std;

// File paths for CSV data

// Update these paths as necessary for your environment
string userDataPath = "/Users/sarahhill/Documents/Workspaces/Algorithms & Data Structures/EduConnect/v1/data/user.csv";
string tutorsDataPath = "/Users/sarahhill/Documents/Workspaces/Algorithms & Data Structures/EduConnect/v1/data/tutors.csv";
string requestsDataPath = "/Users/sarahhill/Documents/Workspaces/Algorithms & Data Structures/EduConnect/v1/data/requests.csv";
string sessionsDataPath = "/Users/sarahhill/Documents/Workspaces/Algorithms & Data Structures/EduConnect/v1/data/sessions.csv";
string subjectsDataPath = "/Users/sarahhill/Documents/Workspaces/Algorithms & Data Structures/EduConnect/v1/data/subjects.csv";
string ratingsDataPath = "/Users/sarahhill/Documents/Workspaces/Algorithms & Data Structures/EduConnect/v1/data/ratings.csv";
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

// ----------------- CSV User Loaders -----------------
vector<User> loadUsersCSV()
{
    vector<User> users;
    ifstream file(userDataPath);
    if (!file.is_open())
    {
        cerr << "Cannot open " << userDataPath << endl;
        return users;
    }

    string line;
    bool firstLine = true;
    while (getline(file, line))
    {
        if (firstLine)
        {
            firstLine = false;
            continue;
        }
        stringstream ss(line);
        User u;
        string temp;
        getline(ss, u.username, ',');
        getline(ss, u.firstName, ',');
        getline(ss, u.lastName, ',');
        getline(ss, u.password, ',');
        getline(ss, u.email, ',');
        getline(ss, u.securityQ, ',');
        getline(ss, u.securityA, ',');
        getline(ss, temp, ',');
        u.userType = stoi(temp);
        users.push_back(u);
    }
    return users;
}

void saveUsersCSV(const vector<User> &users)
{
    ofstream file(userDataPath, ios::trunc); // Clear + rewrite

    if (!file.is_open())
    {
        cerr << "ERROR: Could not open users CSV for writing!" << endl;
        return;
    }

    file << "Username,FirstName,LastName,Password,email,SecurityQ,SecurityA,UserType\n";

    for (const auto &u : users)
    {
        file << u.username << ","
             << u.firstName << ","
             << u.lastName << ","
             << u.password << ","
             << u.email << ","
             << u.securityQ << ","
             << u.securityA << ","
             << u.userType << "\n";
    }

    file.close();
}

// ----------------- CSV Tutor Loaders -----------------
vector<Tutor> loadTutorsCSV()
{
    vector<Tutor> tutors;
    ifstream file(tutorsDataPath);
    if (!file.is_open())
    {
        cerr << "Cannot open " << tutorsDataPath << endl;
        return tutors;
    }

    string line;
    bool firstLine = true;
    while (getline(file, line))
    {
        if (firstLine)
        {
            firstLine = false;
            continue;
        }
        stringstream ss(line);
        Tutor t;
        string subjects;
        string temp;
        getline(ss, temp, ',');
        t.tutorID = stoi(temp);
        getline(ss, t.name, ',');
        getline(ss, t.username, ',');
        getline(ss, subjects, ',');
        getline(ss, t.email, ',');
        getline(ss, temp, ',');
        t.rating = stod(temp);
        getline(ss, temp, ',');
        t.available = (temp == "true");
        stringstream substream(subjects);
        string sub;
        while (getline(substream, sub, ';'))
            t.subjects.push_back(sub);
        tutors.push_back(t);
    }
    return tutors;
}

void saveTutorsCSV(const vector<Tutor> &tutors)
{
    ofstream file(tutorsDataPath, ios::trunc); // Clear + rewrite

    if (!file.is_open())
    {
        cerr << "ERROR: Could not open tutors CSV for writing!" << endl;
        return;
    }

    file << "tutorID,name,username,subjects,email,rating,available\n";

    for (const auto &t : tutors)
    {
        file << t.tutorID << ","
             << t.name << ","
             << t.username << ",";
        for (size_t i = 0; i < t.subjects.size(); ++i)
        {
            file << t.subjects[i];
            if (i < t.subjects.size() - 1)
                file << ";";
        }
        file << "," << t.email << ","
             << t.rating << ","
             << (t.available ? "true" : "false") << "\n";
    }

    file.close();
}

Node *buildTutorTreeFromCSV()
{
    vector<Tutor> tutors = loadTutorsCSV(); // loads from CSV
    Node *root = nullptr;

    for (const Tutor &t : tutors)
    {
        root = insertNode(root, t.tutorID);
    }

    return root;
}

// ----------------- CSV Request Loaders -----------------
vector<Request> loadRequestsCSV()
{
    vector<Request> requests;
    ifstream file(requestsDataPath);
    if (!file.is_open())
    {
        cerr << "Cannot open " << requestsDataPath << endl;
        return requests;
    }

    string line;
    bool firstLine = true;
    while (getline(file, line))
    {
        if (firstLine)
        {
            firstLine = false;
            continue;
        }
        stringstream ss(line);
        Request r;
        string temp;
        getline(ss, temp, ',');
        r.requestID = stoi(temp);
        getline(ss, r.studentUsername, ',');
        getline(ss, r.subject, ',');
        getline(ss, temp, ',');
        r.urgency = stoi(temp);
        getline(ss, r.description, ',');
        requests.push_back(r);
    }
    return requests;
}

void saveRequestsCSV(const vector<Request> &requests)
{
    ofstream file(requestsDataPath, ios::trunc);
    if (!file.is_open())
    {
        cerr << "Cannot open requests CSV for writing!\n";
        return;
    }
    file << "requestID,studentUsername,subject,urgency,description\n"; // correct header
    for (const auto &r : requests)
    {
        file << r.requestID << ","
             << r.studentUsername << ","
             << r.subject << ","
             << r.urgency << ","
             << r.description << "\n";
    }
    file.close();
}

void removeRequestFromCSV(int requestID)
{
    ifstream inFile(requestsDataPath);
    if (!inFile.is_open())
    {
        cerr << "Cannot open file!\n";
        return;
    }

    string header;
    getline(inFile, header); // store header to preserve it
    vector<string> lines;
    string line;

    while (getline(inFile, line))
    {
        lines.push_back(line);
    }
    inFile.close();

    // Remove the line with the matching requestID
    lines.erase(
        remove_if(lines.begin(), lines.end(),
                  [requestID](const string &l)
                  {
                      stringstream ss(l);
                      string firstCol;
                      getline(ss, firstCol, ',');
                      try
                      {
                          return stoi(firstCol) == requestID;
                      }
                      catch (...)
                      {
                          return false;
                      }
                  }),
        lines.end());

    // Rewrite the file with header
    ofstream outFile(requestsDataPath, ios::trunc);
    if (!outFile.is_open())
    {
        cerr << "Cannot open file for writing!\n";
        return;
    }
    outFile << header << "\n";
    for (const auto &l : lines)
        outFile << l << "\n";
    outFile.close();
}

MinHeap *buildRequestHeapFromCSV()
{
    vector<Request> requests = loadRequestsCSV(); // loads from CSV
    MinHeap *heap = new MinHeap(100);             // assuming max 100 requests

    for (const Request &r : requests)
    {
        heap->insert(r.requestID); // using requestID as the key
    }

    return heap;
}

// ----------------- CSV Session History Loaders -----------------
SessionHistory loadSessionsCSV()
{
    SessionHistory history;
    ifstream file(sessionsDataPath);
    if (!file.is_open())
    {
        cerr << "Cannot open " << sessionsDataPath << endl;
        return history;
    }

    string line;
    bool firstLine = true;
    while (getline(file, line))
    {
        if (firstLine)
        {
            firstLine = false;
            continue;
        }
        stringstream ss(line);
        int id;
        string subject, description, tutorUsername, studentUsername, status;
        string temp;
        getline(ss, temp, ',');
        id = stoi(temp);
        getline(ss, subject, ',');
        getline(ss, description, ',');
        getline(ss, tutorUsername, ',');
        getline(ss, studentUsername, ',');
        getline(ss, temp, ','); int requestID = stoi(temp);
        getline(ss, status, ',');
        Session s{id, subject, description, tutorUsername, studentUsername, requestID, status};
        history.addSession(s);
    }
    return history;
}

vector<Session> toVector(const SessionHistory &history) {
    vector<Session> sessions;
    SessionNode *current = history.head;

    while (current) {
        sessions.push_back(current->data);
        current = current->next;
    }
    return sessions;
}


void saveSessionsCSV(const SessionHistory &history)
{
    vector<Session> sessions = toVector(history);

    // Sort sessions by sessionID
    sort(sessions.begin(), sessions.end(), [](const Session &a, const Session &b) {
        return a.sessionID < b.sessionID;
    });

    ofstream file(sessionsDataPath, ios::trunc);
    if (!file.is_open())
    {
        cerr << "Cannot open sessions CSV for writing!\n";
        return;
    }

    file << "sessionID,subject,description,tutorUsername,studentUsername,requestID,status\n";

    for (const auto &s : sessions) {
        file << s.sessionID << ","
             << s.subject << ","
             << s.description << ","
             << s.tutorUsername << ","
             << s.studentUsername << ","
             << s.requestID << ","
             << s.status << "\n";
    }

    file.close();
}


// ----------------- CSV Subject Loaders -----------------
const vector<string> loadSubjectsCSV()
{
    vector<string> subjects;
    ifstream file(subjectsDataPath);
    if (!file.is_open())
    {
        cerr << "Cannot open " << subjectsDataPath << endl;
        return subjects;
    }

    string line;
    bool firstLine = true;
    while (getline(file, line))
    {
        if (firstLine)
        {
            firstLine = false;
            continue;
        }
        subjects.push_back(line);
    }
    return subjects;
}

void SubjectsList()
{
    vector<string> subjects = loadSubjectsCSV();
    cout << "Available Subjects:\n";
    for (const auto &subject : subjects)
    {
        cout << "- " << subject << endl;
    }
}

// ----------------- CSV Ratings Loaders -----------------
vector<Rating> loadRatingsCSV()
{
    vector<Rating> ratings;
    ifstream file(ratingsDataPath);
    if (!file.is_open())
    {
        cerr << "Cannot open " << ratingsDataPath << endl;
        return ratings;
    }

    string line;
    bool firstLine = true;
    while (getline(file, line))
    {
        if (firstLine)
        {
            firstLine = false;
            continue;
        }
        stringstream ss(line);
        Rating r;
        string temp;
        getline(ss, temp, ',');
        r.ratingID = stoi(temp);
        getline(ss, r.tutorUsername, ',');
        getline(ss, r.studentUsername, ',');
        getline(ss, temp, ',');
        r.ratingValue = stoi(temp);
        ratings.push_back(r);
    }
    return ratings;
}

void saveRatingsCSV(const vector<Rating> &ratings)
{
    ofstream file(ratingsDataPath, ios::trunc); // Clear + rewrite

    if (!file.is_open())
    {
        cerr << "ERROR: Could not open ratings CSV for writing!" << endl;
        return;
    }

    file << "ratingID,tutorUsername,studentUsername,ratingValue\n";

    for (const auto &r : ratings)
    {
        file << r.ratingID << ","
             << r.tutorUsername << ","
             << r.studentUsername << ","
             << r.ratingValue << "\n";
    }

    file.close();
}