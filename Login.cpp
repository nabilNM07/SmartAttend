#include "Login.h"
#include <iostream>
#include <string>
using namespace std;

bool Login::authenticate() {
    string username, password;
    const string correctUsername = "admin";
    const string correctPassword = "admin123";

    cout << "\n";

    cout << "====================================\n";
    cout << "           SmartAttend              \n";
    cout << "     Digital Attendance System      \n";
    cout << "====================================\n";
    cout << "          TEACHER LOGIN          \n";
    cout << "====================================\n";

    for(int attempt=0; attempt<3; attempt++) {
        cout << "\n";
        cout << "Enter your username: ";
        cin >> username;
        cout << "Enter your password: ";
        cin >> password;

        if(username == correctUsername && password == correctPassword) {
            cout << "\n";
            cout << "Login successful!\n";
            return true;
    }
    cout << "Invalid username or password. Please try again.\n";
    if(attempt<2) {
        cout << "\n";
        cout << "Attempts remaining: " << 2 - attempt << "\n";
    }
}
    cout << "\nToo many failed attempts."<< endl;
    cout << "Access denied.\n";
    return false;

}
