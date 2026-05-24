#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>
#include "ClearScreen.h"
#include "Decode.h"

// This file contains functions for searching tutors by various criteria

using namespace std;

// I didn't really need to implement any classes for this, so the header file just stores all the searching algorithms
// Saves space on the main space, sorta.

void SearchTutorID()
{
	clearScreen(); // Re-wipe the screen
	cout << "--------------------------------" << endl;
	cout << "      Tutor Search Wizard" << endl;
	cout << "--------------------------------" << endl;
	cout << "Please input which tutor ID you wish to retrieve:" << endl;
	cout << "Tutor ID #";

    int inputId;
    cin>> inputId;

    vector<Tutor> tutors = loadTutorsCSV();
    bool found = false;

    unordered_map<int, Tutor> tutorByID;
    for (const Tutor& t : tutors)
        tutorByID[t.tutorID] = t;

    if (tutorByID.find(inputId) != tutorByID.end()) {
        const Tutor& t = tutorByID[inputId];
        cout << endl << "Tutor #" << t.tutorID << endl;
        cout << "----------" << endl;
        cout << "Name: " << t.name << endl;
        cout << "Subjects: ";
        for (const string& sub : t.subjects) {
            cout << sub << " ";
        }
        cout << endl;
        cout << "Rating: " << t.rating << endl;
        cout << "Email: " << t.email << endl;
        cout << "Available: " << (t.available ? "Yes" : "No") << endl;
        found = true;
    }  
    
    if (!found) {
        cout << "No Tutor exists under this ID." << endl;
    }
}

void SearchSubject() // All the other search algorithms are some modification of the ID one, with some caveats:
{
	clearScreen(); // Re-wipe the screen
	cout << "--------------------------------" << endl;
	cout << "      Tutor Search Wizard" << endl;
	cout << "--------------------------------" << endl;
	cout << "Please select which subject you wish to see tutors for:" << endl;

    vector<string> subjects = loadSubjectsCSV(); // Load subjects from CSV
    vector<Tutor> tutors = loadTutorsCSV();

    int index = 1;
	for (string s : subjects)
    {
        cout << index << ". " << s << endl;
        index++;
    }
    
	int input = 0; // Chooses which subject the user wants to see tutors for 
	string Subject; // The subject the user wants via the options provided
    cin >> input;

	cin.clear(); // As usual, prevent any input carrying into another

   if (input >= 1 && input <= static_cast<int>(subjects.size())) { Subject = subjects[input - 1]; }// Directly assigns our subject to be found to be the same format as used in the CSV file
    else { cout << "Error: Invalid input entered." << endl; }

	cout << endl << "Selected Subject: " << Subject << endl; // Just segments the subject chosen and tutors to be printed
	cout << "--------------------------------" << endl;
    
    unordered_map<string, vector<Tutor*>> tutorsBySubject;
    for (Tutor& t : tutors) {
        for (const string& sub : t.subjects)
            tutorsBySubject[sub].push_back(&t);
    }

    if (tutorsBySubject.find(Subject) != tutorsBySubject.end()) // And we compare if they match. If so, we return the tutor's details
    {
        for (Tutor* t : tutorsBySubject[Subject])
        {
            cout << endl << "Tutor #" << t->tutorID << endl;
            cout << "----------" << endl;
            cout << "Name: " << t->name << endl;
            cout << "Rating: " << t->rating << endl;
            cout << "Email: " << t->email << endl;
            continue; // Since multiple tutors can teach one subject, this function should run until it reaches the end of the table, retrieving every valid tutor as well.
        }
    }
    
}

void SearchTutorName() // All the other search algorithms are some modification of the ID one, with some caveats:
{
	clearScreen(); // Re-wipe the screen
	cout << "--------------------------------" << endl;
	cout << "      Tutor Search Wizard" << endl;
	cout << "--------------------------------" << endl;
	cout << "Please input what tutor name you desire:" << endl;
	cout << "Name: "; // This time, we want this to be able to retrieve either a first name or last name
	string input; // Chooses which subject the user wants to see tutors for 
	bool verifier = false; // Still exists in the event no tutors exist for a given name input
	cin >> input;
	cin.clear(); // As usual, prevent any input carrying into another

	cout << endl << "Selected Tutor(s): " << endl; // Just segments the name chosen and tutors to be printed
	cout << "--------------------------------" << endl;
    
    vector<Tutor> tutors = loadTutorsCSV();

    for (const Tutor& t : tutors)
    {
        // We go through all three cases of what a user can input to find a tutor's name
        if (t.name == input || 
            t.name.substr(0, t.name.find(' ')) == input || 
            t.name.substr(t.name.find(' ') + 1) == input) 
        {
            cout << endl << "Tutor #" << t.tutorID << endl;
            cout << "----------" << endl;
            cout << "Name: " << t.name << endl;
            cout << "Subjects: ";
            for (const string& sub : t.subjects) {
                cout << sub << " ";
            }
            cout << endl;
            cout << "Rating: " << t.rating << endl;
            cout << "Email: " << t.email << endl;
            verifier = true;
            continue; // Since multiple tutors could share a name, this function should run until it reaches the end of the table, retrieving every valid tutor as well.
        }
    }
    if (verifier == false) // If still false
    {
        cout << "No Tutor exists under this name." << endl;
    }
}

void MinTutorRating() // All the other search algorithms are some modification of the ID one, with some caveats:
{
	clearScreen(); // Re-wipe the screen
	cout << "--------------------------------" << endl;
	cout << "      Tutor Search Wizard" << endl;
	cout << "--------------------------------" << endl;
	cout << "Please input what minimum rating you desire for a tutor:" << endl;
	cout << "Min. Rating Threshold: ";
	double input = 0; // Allow decimal values to be used!
	bool verifier = false; // Just ensures if there is a tutor that can meet the threshold
	cin >> input;
	cin.clear(); // As usual, prevent any input carrying into another
	cout << endl << "Selected Tutor(s): " << endl; // Just segments the rating piece and tutors to be printed
	cout << "--------------------------------" << endl;

	vector<Tutor> tutors = loadTutorsCSV();

    for (const Tutor& t : tutors)
    {
        if (t.rating >= input) // We go through all three cases of what a user can input to find a tutor's name
        {
            cout << endl << "Tutor #" << t.tutorID << endl;
            cout << "----------" << endl;
            cout << "Name: " << t.name << endl;
            cout << "Subjects: ";
            for (const string& sub : t.subjects) {
                cout << sub << " ";
            }
            cout << endl;
            cout << "Rating: " << t.rating << endl;
            cout << "Email: " << t.email << endl;
            verifier = true;
            continue; // Since multiple tutors could share a name, this function should run until it reaches the end of the table, retrieving every valid tutor as well.
        }
    }
    if (verifier == false) // If still false
    {
        cout << "No Tutor exists meeting this rating threshold." << endl;
    }
}

void TutorSearchMenu()
{
	clearScreen(); // Assuming the user will access this from a separate window, clear the prior one
	cout << "--------------------------------" << endl;
	cout << "      Tutor Search Wizard" << endl;
	cout << "--------------------------------" << endl;
	cout << "Please select which method you want to search for your tutor(s):" << endl;
	cout << "1. By the Tutor's ID" << endl;
	cout << "2. By the Tutor's Name" << endl;
	cout << "3. By Subject" << endl;
	cout << "4. By Rating Threshold" << endl;
	cout << "Option: ";
	int input = 0; // Let the user select which variation they want to search for a tutor
	cin >> input;
	cin.clear(); // As usual, prevent any input carrying into another
	switch (input)
	{
		case 1: { SearchTutorID(); break; } // Selected Tutor ID search method
		case 2: { SearchTutorName(); break; } // Selected the subject search method
		case 3: { SearchSubject(); break; } // Selected finding tutors by a minimum rating threshold
		case 4: { MinTutorRating(); break;} // Selected finding tutors by a minimum rating threshold
		default: { cout << "Error: Invalid input entered." << endl; } // If an invalid input has been given:
	}
}