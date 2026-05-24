#include "backend/Decode.h"
#include "backend/Authorization.h"
#include "backend/ClearScreen.h"
#include "backend/BinarySearchTree.h"
#include "backend/RequestHeap.h"
#include "backend/RequestProcess.h"
#include "backend/MakeRequest.h"
#include "backend/CreateAccount.h"
#include "backend/TutorSearch.h"
#include "backend/SessionHistory.h"
#include "backend/AcceptRequest.h"
#include "backend/GiveRating.h"
#include "backend/EditProfile.h"


int main() {
    // Before compiling ensure that the file paths in Decode.h are correct for your environment

    vector<User> users = loadUsersCSV();
    //vector<Tutor> tutors = loadTutorsCSV();
    //vector<Request> requests = loadRequestsCSV();
    //SessionHistory sessions = loadSessionsCSV();
    //vector<string> subjects = loadSubjectsCSV();

    // Debug output to verify loading functions
    //cout << "Loaded " << users.size() << " users.\n";
    //cout << "Loaded " << tutors.size() << " tutors.\n";
    //cout << "Loaded " << requests.size() << " requests.\n";
    //cout << "Loaded " << subjects.size() << " subjects.\n";
    
    cout << "--------------------------------" << endl;    
    cout << "     Welcome to EduConnect!" << endl;
    cout << "--------------------------------" << endl;
    cout << "Developed by ScholarStake" << endl << endl;
    cout << "1. Login" << endl;
    cout << "2. Create Account" << endl;
    cout << "Please select an option: ";

    int choice;
    cin >> choice;
    cin.ignore(); 
    if (choice == 1) {
        password(users);
    } else if (choice == 2) {
        makeAccount();
    } else {
        cout << "Invalid choice. Exiting." << endl;
    }

    // Following functions are for testing purposes only, they have been integrated into dashboards
    //SubjectsList(); // Display list of subjects 

    //processRequests(requests, tutors); // Process and match requests to tutors

    //makeAccount(); // Create a new user account (tutor or student)
    //TutorSearchMenu(); // Search for tutors by ID or subject
    //sessions.display(); // Display session history

    //ViewRequests(); // View requests as a tutor
    //AcceptRequest(); // Accept a request as a tutor

    //password(users); // User login and dashboard access
    //makeAccount();
    //viewTutorProfile("tutor123"); // View tutor profile
    //editProfile("tutor123"); // Edit tutor profile

    return 0;
}


