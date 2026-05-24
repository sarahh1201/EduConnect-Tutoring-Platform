# EduConnect – Tutoring Platform

EduConnect is a C++ tutoring platform that connects students with tutors using a custom matching system based on subject, availability, and rating scores. The project demonstrates the use of data structures, algorithms, and system design concepts, with both a command-line backend version and a Qt-based graphical interface.

---

# Project Overview

The project is divided into two main versions:

## CLI Version (Core Backend System)

The CLI version contains the full backend logic of the EduConnect platform, including user management, request handling, and tutor matching algorithms.

### Features
- User registration and authentication (student/tutor accounts)
- Student request creation system
- Tutor request acceptance workflow
- Tutor search and filtering system
- Matching algorithm based on subject, availability, and rating
- CSV-based data storage system

### Backend Modules
- `AcceptRequest.h` – Tutor request acceptance logic  
- `Authorization.h` – Login, authentication, and password management  
- `CreateAccount.h` – Account creation system  
- `Dashboards.h` – Student and tutor menu interfaces  
- `MakeRequest.h` – Student request submission  
- `RequestProcess.h` – Core matching algorithm  
- `TutorSearch.h` – Tutor search and contact functionality  
- `Decode.h` – CSV data parsing and loading  
- `ClearScreen.h` – Terminal UI utility functions  

---

## Qt GUI Version (Graphical Interface)

The Qt version builds on the CLI backend and provides a graphical user interface for interacting with the system.

### Notes
- Built using **Qt (C++ framework)**
- Translates backend logic into UI-based interaction
- Some features were not fully ported due to time constraints
- Uses Qt signal/slot system for UI interaction

---

# How to Run the Project

## Requirements
Before running the project, ensure you have:

- C++ compiler (GCC, MinGW, or MSVC)
- Qt 5 or Qt 6 (ONLY required for GUI version)
- Qt Creator (recommended for GUI version)

---

## Run CLI Version (No Qt Required)

1. Navigate to the CLI version folder  
2. Compile the project:

```bash
g++ main.cpp -o EduConnect