#include "Attendance.h"
#include "Login.h"
#include "Student.h"

#include <iostream>
#include <limits>
using namespace std;

void showMainMenu()
{
    cout << "\n";
    cout << "====================================\n";
    cout << "           SmartAttend              \n";
    cout << "     Digital Attendance System      \n";
    cout << "====================================\n";

    cout << "1. Student Management\n";
    cout << "2. Course Management\n";
    cout << "3. Take Attendance\n";
    cout << "4. View Attendance Records\n";
    cout << "5. Individual Student Report\n";
    cout << "6. Exit\n";

    cout << "====================================\n";
}

int main()
{
    Login login;
    Attendance attendance;
    StudentManager studentManager;

    if (!login.authenticate())
    {
        return 0;
    }

    int choice;

    while (true)
    {
        showMainMenu();
        cout << "\nEnter your choice: ";

        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cout << "\nInvalid input! Please enter a number.\n";
            continue;
        }

        switch (choice)
        {
        case 1:
            studentManager.showStudentMenu();
            break;

        case 2:
            cout << "\nCourse Management selected.\n";
            cout << "This feature is under development.\n";
            break;

        case 3:
            attendance.takeAttendance();
            break;

        case 4:
            attendance.viewAttendanceRecords();
            break;

        case 5:
            attendance.viewStudentAttendanceReport();
            break;

        case 6:
            cout << "\nThanks for using SmartAttend!\n";
            cout << "Exiting the program...\n";
            return 0;

        default:
            cout << "\nInvalid choice! Please select 1-6.\n";
        }
    }
    return 0;
}