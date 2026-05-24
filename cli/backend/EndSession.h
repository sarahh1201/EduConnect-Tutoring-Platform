#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "Decode.h"
using namespace std;

// This file contains functions for ending sessions and viewing active/ended sessions

void EndSession(string &username)
{
    int sessionID;
    cout << "Enter Session ID to end: ";
    cin >> sessionID;

    SessionHistory history = loadSessionsCSV();
    SessionNode *current = history.head;
    bool found = false;

    while (current)
    {
        if (current->data.sessionID == sessionID)
        {
            current->data.status = "ended";
            found = true;
            break;
        }
        current = current->next;
    }

    if (current->data.studentUsername == username || current->data.tutorUsername == username)
    {
        if (found)
        {
            // Save updated sessions back to CSV
            ofstream file(sessionsDataPath);
            if (!file.is_open())
            {
                cerr << "Cannot open sessions CSV for writing!\n";
                return;
            }
            SessionNode *curr = history.head;

            file << "sessionID,subject,description,tutorUsername,studentUsername,requestID,status\n";
            while (curr)
            {
                file << curr->data.sessionID << ","
                     << curr->data.subject << ","
                     << curr->data.description << ","
                     << curr->data.tutorUsername << ","
                     << curr->data.studentUsername << ","
                     << curr->data.requestID << ","
                     << curr->data.status << "\n";
                curr = curr->next;
            }

            file.close();
            cout << "Session " << sessionID << " has been ended successfully.\n";
        }
        else
        {
            cout << "Session ID not found.\n";
        }
    }
    else
    {
        cout << "You are not authorized to end this session.\n";
    }
}

void viewActiveSessions(const string &username)
{
    SessionHistory history = loadSessionsCSV();
    SessionNode *current = history.head;
    bool found = false;

    cout << "Active Sessions for: " << username << endl;
    cout << "----------------------------------------" << endl;

    while (current)
    {
        if ((current->data.tutorUsername == username || current->data.studentUsername == username) && current->data.status == "accepted")
        {
            cout << "Session ID: " << current->data.sessionID << endl;
            cout << "----------------------------------------" << endl;
            cout << "Tutor Username: " << current->data.tutorUsername << endl;
            cout << "Student Username: " << current->data.studentUsername << endl;
            cout << "----------------------------------------" << endl;
            cout << "Subject: " << current->data.subject << endl;
            cout << "Description: " << current->data.description << endl;
            cout << "Student Username: " << current->data.studentUsername << endl;
            cout << "Request ID: " << current->data.requestID << endl;
            cout << "Status: " << current->data.status << endl;
            cout << "----------------------------------------" << endl;
            found = true;
        }
        current = current->next;
    }

    if (!found)
    {
        cout << "No active sessions found." << endl;
    }
}

void viewEndedSessions(const string &username)
{
    SessionHistory history = loadSessionsCSV();
    SessionNode *current = history.head;
    bool found = false;

    cout << "Ended Sessions for: " << username << endl;
    cout << "----------------------------------------" << endl;

    while (current)
    {
        if ((current->data.tutorUsername == username || current->data.studentUsername == username) && current->data.status == "ended")
        {
            cout << "Session ID: " << current->data.sessionID << endl;
            cout << "----------------------------------------" << endl;
            cout << "Tutor Username: " << current->data.tutorUsername << endl;
            cout << "Student Username: " << current->data.studentUsername << endl;
            cout << "----------------------------------------" << endl;
            cout << "Subject: " << current->data.subject << endl;
            cout << "Description: " << current->data.description << endl;
            cout << "Student Username: " << current->data.studentUsername << endl;
            cout << "Request ID: " << current->data.requestID << endl;
            cout << "Status: " << current->data.status << endl;
            cout << "----------------------------------------" << endl;
            found = true;
        }
        current = current->next;
    }

    if (!found)
    {
        cout << "No ended sessions found." << endl;
    }
}