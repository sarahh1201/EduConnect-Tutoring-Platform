#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "Decode.h"

using namespace std;

// ----------------- User Verification -----------------
inline User* verifyUser(vector<User>& users, const string& username, const string& password) {
    for (auto& u : users) {
        if (u.username == username && u.password == password)
            return &u;
    }
    return nullptr;
}

// Helper function to find user by username
inline User* findUserByUsername(const string& username, vector<User>& users) {
    for (auto& u : users) {
        if (u.username == username) {
            return &u;   // return pointer to the exact user
        }
    }
    return nullptr; // not found
}


// ----------------- Check Password -----------------
inline bool passwordCheck(const string &username, const string &password, bool &isTutor)
{
    vector<User> users = loadUsersCSV();

    User* user = verifyUser(users, username, password);

    if (user != nullptr) {
        isTutor = (user->userType == 1); // or however you store the type
        return true;
    }

    return false;
}

inline void updateTutorAverage(const std::string &tutorUsername) {
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

inline double partition(vector<Tutor>& tutors, int low, int high) // How we do the array 'split':
{
    double pivotPoint = tutors[high].rating; // Just assign the last variable of the array to be our 'pivot'
    int prev = (low - 1); // Assign 'prev' as the index BEFORE the current index

    for (int cur = low; cur <= high - 1; cur++)
    {
        if (tutors[cur].rating <= pivotPoint) // If the current value is equal or greater than the pivot value
        {
            prev++; // Count the previous value up
            swap(tutors[prev], tutors[cur]); // Real simple 'swap' operation between the two
        }
    }

    swap(tutors[prev + 1], tutors[high]); // When the loop is done, swap the ending two values
    return (prev + 1); // And return this new 'high' value
}

inline void quickSort(vector<Tutor> &tutors, int low, int high) // The real star of the show
{
    if (low < high) // Quite simply, so long as the 'low' value is still 'low', we rerun the quicksort over and over
    {
        int partInd = partition(tutors, low, high); // How to do the partitioning operations, there's a function for that!
        quickSort(tutors, low, partInd - 1); // Left partition goes from the low bounds
        quickSort(tutors, partInd + 1, high); // Right partition goes to the high bounds, partInd is our 'middle' value for both
    }
}



