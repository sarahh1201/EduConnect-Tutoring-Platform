//request.h
#include <iostream>
#include <vector>
#include <queue>
#include <string>
using namespace std;

struct Tutor {
    int tutor_id;
    string name;
    vector<string> subjects;
    double rating;       
    bool available;      
    Tutor(int id, string n, vector<string> subs, double r, bool a)
        : tutor_id(id), name(n), subjects(subs), rating(r), available(a) {
    }
};

struct Request {
    int student_id;
    string subject;
    string urgency;
    string description;

    Request(int sid, string subj, string urg, string desc)
        : student_id(sid), subject(subj), urgency(urg), description(desc) {
    }

    
	double computeScore(const Tutor& tutor) {// compute a score for a tutor based on rating and availability
       
        bool subjectMatch = false;
        for (const auto& s : tutor.subjects) {//match subject and request
            if (s == subject) {
                subjectMatch = true;
                break;
            }
        }
		if (!subjectMatch) return -1.0; // remove invalid tutors

        double ratingScore = tutor.rating / 5.0;      
        double availabilityScore = tutor.available ? 1.0 : 0.0;

		return (0.7 * ratingScore) + (0.3 * availabilityScore);//weight rating and availability
    }

    vector<Tutor> matchTutors(const vector<Tutor>& tutors) {// Matching algorithm using priority queue
        auto cmp = [](pair<double, Tutor> left, pair<double, Tutor> right) {
            return left.first < right.first; // larger scores are higher priority
            };
        priority_queue<pair<double, Tutor>,
            vector<pair<double, Tutor>>,
            decltype(cmp)> heap(cmp);

        for (const auto& tutor : tutors) {
            double score = computeScore(tutor);
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
}; 
