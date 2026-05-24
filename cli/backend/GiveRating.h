#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include "Decode.h"
using namespace std;

// This file contains functions for giving ratings to tutors and updating their average ratings

void updateTutorAverage(const std::string &tutorUsername) {
    vector<Rating> ratings = loadRatingsCSV();
    int total = 0;
    int count = 0;  
    for (const auto &r : ratings) {
        if (r.tutorUsername == tutorUsername) {
            total += r.ratingValue;
            count++;
        }
    }

    double avg = (count == 0) ? 0 : static_cast<double>(total) / count;

    vector<Tutor> tutors = loadTutorsCSV();

    for (auto &t : tutors) {
        if (t.username == tutorUsername) {
            t.rating = avg;
            break;
        }
    }

    saveTutorsCSV(tutors);
}


void viewPendingRatings(const string& studentUsername) {
    SessionHistory history = loadSessionsCSV();
    SessionNode* current = history.head;
    bool found = false;

    cout << "Sessions pending ratings for: " << studentUsername << endl;
    cout << "----------------------------------------" << endl;

    while (current) {
        if (current->data.studentUsername == studentUsername &&
            current->data.status == "ended") {
            cout << "Session ID: " << current->data.sessionID
                 << ", Tutor: " << current->data.tutorUsername
                 << ", Subject: " << current->data.subject
                 << ", Description: " << current->data.description << endl;
            found = true;
        }
        current = current->next;
    }

    if (!found) {
        cout << "No sessions pending ratings." << endl;
    }
}

void GiveRating(const string& studentUsername) {
    // Load session history once
    SessionHistory history = loadSessionsCSV();

    // Display pending sessions
    SessionNode* current = history.head;
    viewEndedSessions(studentUsername);
    
    // Ask for tutor username
    cout << "Enter the tutor's username you want to rate: ";
    string tutorUsername;
    cin >> tutorUsername;
    cin.ignore();

    if (current->data.tutorUsername != tutorUsername)
    {
       cout << "No ended session found with tutor " << tutorUsername << " for student " << studentUsername << "." << endl;
       return;
    }
    

    int ratingValue;
    string comments;
    cout << "Enter rating for tutor (1-5): ";
    cin >> ratingValue;
    cin.ignore();
    cout << "Enter comments: ";
    getline(cin, comments);

    // Append rating to CSV
    vector<Rating> ratings = loadRatingsCSV();
    int nextRatingID = ratings.empty() ? 1 : ratings.back().ratingID + 1;
    ofstream file(ratingsDataPath, ios::app);
    if (!file.is_open()) {
        cerr << "Cannot open ratings CSV!" << endl;
        return;
    }
    file << nextRatingID << "," << tutorUsername << "," << studentUsername
         << "," << ratingValue << "," << comments << "\n";
    file.close();
    cout << "Rating submitted successfully." << endl;

    updateTutorAverage(tutorUsername);

    // Update session status
    current = history.head;
    while (current) {
        if (current->data.studentUsername == studentUsername &&
            current->data.tutorUsername == tutorUsername &&
            current->data.status == "ended") {
            current->data.status = "reviewed";
            break; // update only the intended session
        }
        current = current->next;
    }

    saveSessionsCSV(history);
    cout << "Session status updated to 'reviewed'." << endl;
}

