#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "Decode.h"
#include "ClearScreen.h"
#include "MakeRequest.h"
#include "TutorSearch.h"
#include "EndSession.h"
#include "GiveRating.h"
#include "EditProfile.h"
#include "QuickSort.h"
#include "RequestProcess.h"
#include "AcceptRequest.h"

// This file contains functions for displaying and managing user dashboards

using namespace std;

void tutorHeader()
{
    clearScreen_verify();
    cout << "--------------------------------" << endl;
    cout << "        Tutor Dashboard" << endl;
    cout << "--------------------------------" << endl;
}

void studentHeader()
{
    clearScreen_verify();
    cout << "--------------------------------" << endl;
    cout << "       Student Dashboard" << endl;
    cout << "--------------------------------" << endl;
}

void TutorDashboard(string tutorUsername)
{
    int run = 1;
    while (run)
    {
    tutorHeader();

    cout << "Welcome, " << tutorUsername << "!" << endl;

    cout << "1. View Requests" << endl;
    cout << "2. Accept Request" << endl;
    cout << "3. View Active Sessions" << endl;
    cout << "4. End Session" << endl;
    cout << "5. Edit Profile" << endl;
    cout << "6. Logout" << endl;
    cout << "Please select an option: ";

    
    int choice;
    cin >> choice;

    switch (choice)
    {
    case 1:
        clearScreen();
        ViewRequests(tutorUsername);
        break;
    case 2:
        clearScreen();
        ViewRequests(tutorUsername);
        AcceptRequest(tutorUsername);
        break;
    case 3:
        clearScreen();
        viewActiveSessions(tutorUsername);
        break;
    case 4:
        clearScreen();
        viewActiveSessions(tutorUsername);
        EndSession(tutorUsername);
        break;
    case 5:
        clearScreen();
        editProfileTutor(tutorUsername);
        break;
    case 6:
        cout << "Logging out..." << endl;
        run = 0;
        return;
    default:
        cout << "Invalid choice. Please try again." << endl;
    }
}
}

void StudentDashboard(string studentUsername)
{
    int run = 1;
    while (run)
    {
    studentHeader();
    cout << "Welcome, " << studentUsername << "!" << endl;

    cout << "1. Make a Request" << endl;
    cout << "2. View Suggested Tutors" << endl;
    cout << "3. Search for Tutors" << endl;
    cout << "4. Active Sessions" << endl;
    cout << "5. Give Rating" << endl;
    cout << "6. End Session" << endl;
    cout << "7. Edit Profile" << endl;
    cout << "8. Logout" << endl;
    cout << "Please select an option: ";

    int choice;
    cin >> choice;

    switch (choice)
    {
    case 1:
        clearScreen();
        makeRequest(studentUsername);
        break;
    case 2:
        clearScreen();
        suggestTutorsForStudent(studentUsername);
        break;
        
    case 3:
        clearScreen();
        TutorSearchMenu();
        break;
    case 4:
        clearScreen();
        viewActiveSessions(studentUsername);
        break;
    case 5:
        clearScreen();
        GiveRating(studentUsername);
        break;
    case 6:
        clearScreen();
        viewActiveSessions(studentUsername);
        EndSession(studentUsername);
        break;
    
    case 7:
        clearScreen();
        editProfileStudent(studentUsername);
        break;
    case 8:
        cout << "Logging out..." << endl;
        run = 0;
        return;
    default:
        cout << "Invalid choice. Please try again." << endl;
    }
}
}

void adminDashboard()
{
    int run = 1;
    while (run)
    {
    
    clearScreen_verify();
    cout << "--------------------------------" << endl;
    cout << "        Admin Dashboard" << endl;
    cout << "--------------------------------" << endl;

    cout << "Welcome, Admin!" << endl;
    cout << "1. View All Tutors" << endl;
    cout << "2. View the top Tutors" << endl;
    cout << "3. View All Students" << endl;
    cout << "4. View Session History" << endl;
    cout << "5. Process Requests" << endl;
    cout << "6. Logout" << endl;
    cout << "Please select an option: ";

    int choice;
    cin >> choice;
    switch (choice)
    {
    case 1:
        clearScreen();
        {
            int count = 0;
            vector<Tutor> tutors = loadTutorsCSV();
            cout << "All Tutors:" << endl;
            for (const auto &t : tutors)
            {
                cout << "Name: " << t.name << ", Username: " << t.username << ", Rating: " << t.rating << endl;
                count++;
            }   
            cout << "Total Tutors: " << count << endl;
        }
        break;
    case 2:
        clearScreen();
        {
            vector<Tutor> tutors = loadTutorsCSV();

            quickSort(tutors, 0, tutors.size() - 1);
            cout << "Top Tutors:" << endl;
            for (size_t i = 0; i < min(tutors.size(), size_t(3)); i++)
            {
                const auto &t = tutors[tutors.size() - 1 - i];
                cout << (i+1) << ". Name: " << t.name << ", Username: " << t.username << ", Rating: " << t.rating << endl;
            }
        }
        break;
    case 3:
        clearScreen();
        {
            int count = 0;
            vector<User> users = loadUsersCSV();
            cout << "All Students:" << endl;
            for (const auto &s : users)
            {
                if (s.userType == 0)
                {
                    cout << "Name: " << s.firstName << " " << s.lastName << ", Username: " << s.username << endl;
                    count++;
                }
            }
            cout << "Total Students: " << count << endl;
        }
        break;
    case 4:
        clearScreen();
        {
            SessionHistory sessions = loadSessionsCSV();
            cout << "Session History:" << endl;
            sessions.display();
        }
        break;
    case 5:
        clearScreen();
        {
            vector<Request> requests = loadRequestsCSV();
            vector<Tutor> tutors = loadTutorsCSV();
            processRequests(requests, tutors);
        }
        break;
    case 6:
        cout << "Logging out..." << endl;
        run = 0;
        return;
    default:
        cout << "Invalid choice. Please try again." << endl;
    }
}
}   