#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "Decode.h"
#include "ClearScreen.h"
#include "Dashboards.h"

// This file contains functions for user authorization, including login, password recovery, and password changing.

using namespace std;

// ----------------- User Verification -----------------
User* verifyUser(vector<User>& users, const string& username, const string& password) {
    for (auto& u : users) {
        if (u.username == username && u.password == password)
            return &u;
    }
    return nullptr;
}

// Helper function to find user by username
User* findUserByUsername(const string& username, vector<User>& users) {
    for (auto& u : users) {
        if (u.username == username) {
            return &u;   // return pointer to the exact user
        }
    }
    return nullptr; // not found
}

// ----------------- Password Recovery -----------------
void PasswordRecovery(vector<User>& users, const string& user) // Asks the user a security question, and if they get it correct present the current password in the 'server'
{
	User* userPtr = findUserByUsername(user, users);

	if (!userPtr) {
		cout << "User not found!" << endl;
		return;
	}
	
	clearScreen(); // Clear the display for this new 'window'
	cout << "--------------------------------------" << endl;
	cout << "    Educonnect Password Recovery:" << endl;
	cout << "--------------------------------------" << endl;
	cout << "Here is your defined security question:" << endl;
	string input; // Just the user's input
    int screwUps = 0; // How long the system will be locked out if you keep mucking it up
	cout << userPtr->securityQ << " ";
	
	while (screwUps < 3) // Max of 3 attempts to get this right
	{
		cout << "Your Answer: ";
		getline(cin, input); // Is able to accept a full line as an answer too
		if (input == userPtr->securityA) // If accepted, show the concurrent password and return
		{
			cout << "Answer accepted. Your password is: " << userPtr->password << endl;
			return;
		}
		else // If not accepted, increment the screw up counter
		{
			cout << "Incorrect answer." << endl;
			screwUps++;
			cout << "Number of attempts remaining: " << (3 - screwUps) << endl;
		}
	}
	cout << "Maximum attempts reached. Please try again later." << endl;	
	return;		
}

// ----------------- Password Changer -----------------
void PasswordChanger(vector<User>& users, const string& user) // Does exactly what's on the tin
{ 
	User* userPtr = findUserByUsername(user, users);

	if (!userPtr) {
		cout << "User not found!" << endl;
		return;
	}

	// NOTE: This function is currently unused, be sure to have an option for the user to change their password when logged in!
	clearScreen(); // Clear way for this cool new window!
	cout << "--------------------------------" << endl;
	cout << "   Educonnect Password Changer" << endl;
	cout << "--------------------------------" << endl;
	cout << "User ID: "; // First, ask the user to re-input their ID once more for verification
	string ID;
	getline(cin, ID); // Quite aptly
	cin.clear(); // This is to prevent any other input just randomly being applied to the next time 'cin' is called

	if (ID != userPtr->username) // If the ID doesn't match up, boot them outta here
	{
		cout << "Error: User ID does not match logged in user." << endl;
		return;
	}

	int screwUps = 0; // Now for the password stage, and for everyone's favourite variable, the screw up counter!
	string oldPassword; // Let this represent the old password the user will be inputting
	
	cout << "Enter your current password: ";
	getline(cin, oldPassword);

	while (screwUps < 3) // Max of 3 attempts to get this right
	{
		if (oldPassword == userPtr->password) // If accepted, move on to the next stage
		{
			string newPassword, confirmPassword; // New password and its confirmation counterpart
			cout << "Enter your new password: ";
			cin >> newPassword;
			cout << "Confirm your new password: ";
			cin >> confirmPassword;

			if (newPassword == confirmPassword) // If both new passwords match, change it!
			{
				userPtr->password = newPassword; // Change the password in the 'server'
				saveUsersCSV(users);
				cout << "Password successfully changed!" << endl;
				return;
			}
			else // If not, inform the user and exit
			{
				cout << "Error: New passwords do not match." << endl;
				return;
			}
		}
		else // If not accepted, increment the screw up counter
		{
			cout << "Incorrect current password." << endl;
			screwUps++;
			cout << "Number of attempts remaining: " << (3 - screwUps) << endl;
			if (screwUps < 3) {
				cout << "Enter your current password: ";
				cin >> oldPassword;
			}
		}
	}
	cout << "Maximum attempts reached. Please try again later." << endl;	
	return;
}

// ----------------- Login Screen -----------------
void password(vector<User>& users) {
    const int maxAttempts = 3;
    int attempts = 0;

	clearScreen(); // Ol' Reliable
	cout << "--------------------------------" << endl;
	cout << "   Educonnect Secure Screen:" << endl;
	cout << "--------------------------------" << endl;

    while (attempts < maxAttempts) {
        string userID, password;
        cout << "Enter User ID: ";
		cin >> userID;
		cin.ignore(); // Clear newline character from buffer
        cout << "Enter Password (Type 'recover' to recover password, 'change' to change password): ";
		cin >> password;
		cin.ignore(); // Clear newline character from buffer

		if (userID == "admin" && password =="admin")
		{
			cout << "Login successful." << endl;
			adminDashboard();
			return; // Exit after admin dashboard
		}
		

        User* user = verifyUser(users, userID, password);
        if (user) {
            cout << "Login successful! Welcome, " 
                 << user->firstName << " " 
                 << user->lastName << "." << endl;

            if (user->userType == 1) {
                cout << "User Type: Tutor" << endl;
				TutorDashboard(user->username);
			}
            else {
                cout << "User Type: Student" << endl;
				StudentDashboard(user->username);
			}
            return; // Exit after successful login
        } else {
			    // Recovery
				if (password =="recover"){
					PasswordRecovery(users,userID);
				}
				// Change Password
				else if (password =="change"){
					PasswordChanger(users,userID);
				}
				else{
					cout << "Invalid User ID or Password. You have " << (maxAttempts - attempts - 1) << " attempts remaining." << endl;
				}
            
            attempts++;
        }
    }

    cout << "Maximum login attempts reached. Exiting..." << endl;
}

