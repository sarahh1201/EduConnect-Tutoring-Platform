#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "Decode.h"

using namespace std;

// This file contains functions for making new tutoring requests

void makeRequest(string studentUsername) {
    Request newRequest;
    newRequest.studentUsername = studentUsername;
    cout << "Enter subject: ";
    cin >> newRequest.subject;
    cout << "Enter urgency (1-5): ";
    cin >> newRequest.urgency;
    cout << "Enter description: ";
    cin.ignore(); // to clear the newline character from the buffer
    getline(cin, newRequest.description);

    // Load existing requests to determine next requestID
    vector<Request> requests = loadRequestsCSV();
    newRequest.requestID = requests.empty() ? 1 : requests.back().requestID + 1;

    // Append new request to CSV
    ofstream file(requestsDataPath, ios::app);
    if (!file.is_open()) {
        cerr << "ERROR: Could not open requests CSV for appending!" << endl;
        return;
    }

    file << newRequest.requestID << ","
         << newRequest.studentUsername << ","
         << newRequest.subject << ","
         << newRequest.urgency << ","
         << newRequest.description << "\n";

    file.close();
    cout << "Request submitted successfully with ID: " << newRequest.requestID << endl;
}