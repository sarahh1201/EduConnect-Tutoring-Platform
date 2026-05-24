#pragma once

#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include "Decode.h"
using namespace std;

inline double computeScore(const Request& req, const Tutor& tutor) {// compute a score for a tutor based on rating and availability
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

inline vector<Tutor> matchTutors(const Request&req, const vector<Tutor>& tutors) {// Matching algorithm using priority queue
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

inline string processRequests(const vector<Request>& requests, const vector<Tutor>& tutors) {
    string request;

    for (const auto& req : requests) {

        request += "Processing Request ID: " + to_string(req.requestID) + "<br>";
        request += "Subject: " + req.subject + "<br>";
        request += "Urgency: " + to_string(req.urgency) + "<br><br>";

        vector<Tutor> matches = matchTutors(req, tutors);

        if (!matches.empty()) {


            request += "<b>Top Matched Tutor:</b><br>";
            request += "Name: " + matches[0].name + "<br>";
            request += "Rating: " + to_string(matches[0].rating) + "<br>";
            request += "Available: " + string(matches[0].available ? "Yes" : "No") + "<br><br>";


            request += "<b>All Matches:</b><br>";
            int count = 1;
            for (const auto& t : matches) {
                request += to_string(count) + ") Tutor: " + t.name
                           + " | Rating: " + to_string(t.rating)
                           + " | Available: " + (t.available ? "Yes" : "No")
                           + "<br>";
                count++;
            }
            request += "<hr>";

        } else {
            request += "No available tutors found for this request.<br><hr>";
        }
    }

    return request;
}

