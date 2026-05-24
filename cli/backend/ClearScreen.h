#pragma once
#include <cstdlib>
#include <iostream>

// This file contains functions for clearing the console screen

using namespace std;
// Function to clear the console screen
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// Asks user to confirm before clearing screen
void clearScreen_verify() {
    cout << "Enter 0 to clear:";
    int in;
    cin >> in;
    if (in==0){
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
    }
}