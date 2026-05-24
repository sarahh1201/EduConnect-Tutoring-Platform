#pragma once

#include "Decode.h"
#include "SessionHistory.h"
#include <iostream>
using namespace std;

// This file contains functions for tutors to view and accept requests

// Helper function to check if tutor teaches a subject
bool tutorTeaches(const Tutor &t, const string &subject)
{
    for (const string &s : t.subjects)
    {
        if (s == subject)
            return true;
    }
    return false;
}

void UpdateRequestStatus(int reqID, const string &tutorUsername)
{
    vector<Request> requests = loadRequestsCSV();

    // Find the request
    auto it = find_if(requests.begin(), requests.end(),
                      [reqID](const Request &r)
                      { return r.requestID == reqID; });
    if (it == requests.end())
    {
        cout << "Request ID not found." << endl;
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

    cout << "Request " << reqID << " accepted and session created.\n";
}

void AcceptRequest(string tutorUsername)
{
    vector<Request> requests = loadRequestsCSV();
    vector<Tutor> tutors = loadTutorsCSV();

    // Find this tutor
    Tutor currentTutor;
    bool foundTutor = false;

    for (const auto &t : tutors)
    {
        if (t.username == tutorUsername)
        {
            currentTutor = t;
            foundTutor = true;
            break;
        }
    }

    if (!foundTutor)
    {
        cout << "Error: Tutor not found.\n";
        return;
    }

    cout << "\nEnter Request ID to accept: ";
    int reqID;
    cin >> reqID;

    for (const auto &r : requests)
    {
        if (r.requestID == reqID)
        {
            // Ensure tutor teaches the requested subject
            if (!tutorTeaches(currentTutor, r.subject))
            {
                cout << "You cannot accept this request — you do NOT teach this subject.\n";
                return;
            }

            // Accept request
            cout << "Request " << reqID << " accepted successfully.\n";
            UpdateRequestStatus(reqID, tutorUsername);  // remove request + add session history
            return;
        }
    }

    cout << "Request ID not found.\n";
}

void ViewRequests(string tutorUsername)
{
    vector<Request> requests = loadRequestsCSV();
    vector<Tutor> tutors = loadTutorsCSV();

    // Find tutor by username
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
        cout << "Error: Tutor not found.\n";
        return;
    }

    cout << "\nRequests you are allowed to accept:\n";

    bool printed = false;
    for (const auto &req : requests)
    {
        if (tutorTeaches(currentTutor, req.subject))
        {
            printed = true;

            cout << "Request ID: " << req.requestID
                 << " | Student: " << req.studentUsername
                 << " | Subject: " << req.subject
                 << " | Urgency: " << req.urgency
                 << " | Description: " << req.description << endl;
        }
    }

    if (!printed)
        cout << "(No requests available for your subjects.)\n";
}