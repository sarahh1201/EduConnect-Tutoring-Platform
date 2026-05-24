#pragma once

#include <iostream>
#include <string>
using namespace std;

// This file contains structures and functions for managing session history

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

    void display() const {
        SessionNode* current = head;
        while (current) {
            cout << "Session " << current->data.sessionID 
                 << ": " << current->data.subject
                 << " - " << current->data.description
                 << " (TutorUsername: " << current->data.tutorUsername
                 << ", StudentUsername: " << current->data.studentUsername
                 << ", RequestID: " << current->data.requestID
                 << ", Status: " << current->data.status << ")"
                 << endl;
            current = current->next;
        }
    }
};