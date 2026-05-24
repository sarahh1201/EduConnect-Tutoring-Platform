#pragma once
#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include "Decode.h"
using namespace std;

// This file contains functions for processing student requests and matching them with tutors

double computeScore(const Request& req, const Tutor& tutor) {// compute a score for a tutor based on rating and availability   
    bool subjectMatch = false;
    for (const auto& s : tutor.subjects) {
        if (s == req.subject) {
            subjectMatch = true;
            break;
        }
    }
    if (!subjectMatch) return -1.0; // remove invalid tutors

    double ratingScore = tutor.rating / 5.0;      
    double availabilityScore = tutor.available ? 1.0 : 0.0;

    return (0.7 * ratingScore) + (0.3 * availabilityScore);//weight rating and availability
}

vector<Tutor> matchTutors(const Request&req, const vector<Tutor>& tutors) {// Matching algorithm using priority queue
    auto cmp = [](pair<double, Tutor> left, pair<double, Tutor> right) {
        return left.first < right.first; // larger scores are higher priority
        };
    priority_queue<pair<double, Tutor>,
        vector<pair<double, Tutor>>,
        decltype(cmp)> heap(cmp);

    for (const auto& tutor : tutors) {
        double score = computeScore(req, tutor);
        if (score >= 0.0) { // only consider tutors with matching subjects
            heap.push({ score, tutor });
        }
    }

    // Extract tutors in ranked order
    vector<Tutor> rankedTutors;
    while (!heap.empty()) {
        rankedTutors.push_back(heap.top().second);
        heap.pop();
    }
    return rankedTutors;
}

void processRequests(const vector<Request>& requests, const vector<Tutor>& tutors) {
    for (const auto& req : requests) {
        cout << "\nProcessing Request ID: " << req.requestID
             << " | Subject: " << req.subject
             << " | Urgency: " << req.urgency << endl;

        vector<Tutor> matches = matchTutors(req, tutors);

        if (!matches.empty()) {
            cout << "Top Matched Tutor: " << matches[0].name
                 << " | Rating: " << matches[0].rating
                 << " | Available: " << (matches[0].available ? "Yes" : "No") << endl;

                 int count = 1;
            for (const auto& t : matches) {//could change availability to set times
            cout << count<<") "<< "Tutor: " << t.name<< " | Rating: " << t.rating<< " | Available: " << (t.available ? "Yes" : "No")<<endl;
            count++;
            }
        } else {
            cout << "No available tutors found for this request.\n";
        }
    }
}

void suggestTutorsForStudent(const string& studentUsername) {
    vector<Request> requests = loadRequestsCSV();
    vector<Tutor> tutors = loadTutorsCSV();
    bool found = false;

    cout << "Suggested Tutors for Student: " << studentUsername << endl;
    

    for (const auto& req : requests) {
        if (req.studentUsername == studentUsername) {
            cout << "\nFor Request ID: " << req.requestID
                 << " | Subject: " << req.subject
                 << " | Urgency: " << req.urgency << endl;

            vector<Tutor> matches = matchTutors(req, tutors);
            found = true;

            if (!matches.empty()) {
                cout << "Top Suggested Tutor: " << matches[0].name
                     << " | Rating: " << matches[0].rating
                     << " | Available: " << (matches[0].available ? "Yes" : "No") << endl;
            } else {
                cout << "No available tutors found for this request.\n";
            }
        }
    }
    if (!found) {
        cout << "No requests found for student: " << studentUsername << endl;
    }
}