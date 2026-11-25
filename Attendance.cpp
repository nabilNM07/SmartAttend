#include "Attendance.h"
#include "FileManager.h"
#include "Student.h"

#include <iostream>
#include <limits>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <string>
using namespace std;

bool isValidDate(const string &date)
{
    //! Checking format------> YYYY-MM-DD
    if (date.length() != 10 ||
        date[4] != '-' ||
        date[7] != '-')
    {
        return false;
    }

    //! characters limit
    for (int i = 0; i < 10; i++)
    {
        if (i == 4 || i == 7)
            continue;

        if (date[i] < '0' || date[i] > '9')
            return false;
    }

    int year =
        (date[0] - '0') * 1000 +
        (date[1] - '0') * 100 +
        (date[2] - '0') * 10 +
        (date[3] - '0');

    int month =
        (date[5] - '0') * 10 +
        (date[6] - '0');

    int day =
        (date[8] - '0') * 10 +
        (date[9] - '0');

    if (year == 0 || month < 1 || month > 12)
        return false;

    int daysInMonth[] = {
        31, 28, 31, 30, 31, 30,
        31, 31, 30, 31, 30, 31};

    //! leap year
    bool leapYear =
        (year % 400 == 0) ||
        (year % 4 == 0 && year % 100 != 0);

    if (leapYear && month == 2)
    {
        daysInMonth[1] = 29;
    }

    return day >= 1 && day <= daysInMonth[month - 1];
}

void Attendance::takeAttendance()
{
    FileManager fileManager;
    //* Load previous attendance records from file
    records = fileManager.loadAttendanceRecords();

    string section, date;

    cout << "\n========Take Attendance========\n";

    cout << "Enter section: ";
    cin >> section;

    //! date validation
    while (true)
    {
        cout << "Enter date (YYYY-MM-DD): ";
        cin >> date;

        if (isValidDate(date))
            break;

        cout << "\nInvalid date! Please enter a valid date.\n";
    }
    //! Load registered students
    vector<Student> students = fileManager.loadStudents();

    //! check for duplicate attendance
    for (const auto &record : records)
    {
        if (record.section == section && record.date == date)
        {
            cout << "\nAttendance already exists for section " << section
                << " on date " << date << "!\n";
            return;
        }
    }

    cout << "\n1. Present";
    cout << "\n2. Absent";
    cout << "\n3. Late\n";

    vector<AttendanceRecord> newRecords;

    bool foundStudent = false;

    for (const auto &student : students)
    {
        if (student.getSection() != section)
            continue;

        foundStudent = true;

        int choice;

        cout << "\nStudent ID: " << student.getId();
        cout << "\nStudent Name: " << student.getName();

        while (true)
        {
            cout << "\nEnter Status (1-3): ";

            if (!(cin >> choice))
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                cout << "\nInvalid input! Please Try again.\n";
                continue;
            }
            if (choice >= 1 && choice <= 3)
                break;

            cout << "\nInvalid choice! Please enter 1, 2, or 3.\n";
        }

        string status;

        if (choice == 1)
            status = "Present";
        else if (choice == 2)
            status = "Absent";
        else if (choice == 3)
            status = "Late";

        AttendanceRecord record;

        record.studentID = student.getId();
        record.studentName = student.getName();
        record.section = student.getSection();
        record.date = date;
        record.status = status;

        newRecords.push_back(record);
    }

    if (!foundStudent)
    {
        cout << "\nNo students found in section "
             << section << "!\n";
        return;
    }

    //! Add new records to the existing records

    size_t oldSize = records.size();

    records.insert(
        records.end(),
        newRecords.begin(),
        newRecords.end());

    //! Save all records permanently
    if (!fileManager.saveAttendanceRecords(records))
    {
        records.resize(oldSize);
        cout << "\nError saving attendance records!\n";
    }

    cout << "\nAttendance taken successfully!\n";
}

void Attendance::viewAttendanceRecords()
{
    FileManager fileManager;

    //! Load saved attendance records
    records = fileManager.loadAttendanceRecords();

    cout << "\n========== ATTENDANCE REPORT ==========\n";

    if (records.empty())
    {
        cout << "No attendance records found.\n";
        return;
    }

    string section;

    cout << "Enter section: ";
    cin >> section;

    //! Temporary structure for student
    struct StudentRow
    {
        int id;
        string name;
    };

    struct LowAttendanceWarning
    {
        int id;
        string name;
        double percentage;
    };

    vector<StudentRow> students;
    vector<string> dates;
    vector<LowAttendanceWarning> warnings;

    //! Collecting students and unique dates
    for (const auto &record : records)
    {
        if (record.section != section)
            continue;

        //! unique dates
        if (find(dates.begin(), dates.end(), record.date) == dates.end())
        {
            dates.push_back(record.date);
        }

        //! Collecting unique students
        bool studentExists = false;

        for (const auto &student : students)
        {
            if (student.id == record.studentID)
            {
                studentExists = true;
                break;
            }
        }

        if (!studentExists)
        {
            students.push_back({record.studentID,
                                record.studentName});
        }
    }

    //! selected section has records or not
    if (students.empty())
    {
        cout << "\nNo attendance records found for section "
            << section << "!\n";

        return;
    }

    //! Sorting dates and students
    sort(dates.begin(), dates.end());

    sort(students.begin(), students.end(),
        [](const StudentRow &a, const StudentRow &b)
        {
            return a.id < b.id;
        });

    //! Displaying date-wise attendance sheet
    cout << "\n========== SECTION " << section
        << " ATTENDANCE SHEET ==========\n\n";

    cout << left
        << setw(12) << "Student ID"
        << setw(18) << "Name";

    for (const auto &date : dates)
    {
        cout << setw(13) << date;
    }

    cout << "\n";
    cout << string(30 + dates.size() * 13, '-') << "\n";

    for (const auto &student : students)
    {
        cout << left
            << setw(12) << student.id
            << setw(18) << student.name;

        for (const auto &date : dates)
        {
            string status = "-";

            for (const auto &record : records)
            {
                if (record.studentID == student.id &&
                    record.section == section &&
                    record.date == date)
                {
                    if (record.status == "Present")
                        status = "P";

                    else if (record.status == "Absent")
                        status = "A";

                    else if (record.status == "Late")
                        status = "L";

                    break;
                }
            }

            cout << setw(13) << status;
        }

        cout << "\n";
    }

    cout << "\nP = Present | A = Absent | L = Late";
    cout << "\n- = No attendance record\n";

    //! --------ATTENDANCE STATISTICS----------
    int totalClasses = dates.size();

    cout << "\n\n========== ATTENDANCE STATISTICS ==========\n";

    cout << "\nTotal Classes: " << totalClasses << "\n\n";

    cout << left
        << setw(10) << "ID"
        << setw(15) << "Name"
        << setw(12) << "Present"
        << setw(12) << "Absent"
        << setw(10) << "Late"
        << setw(14) << "Adj.Present"
        << setw(14) << "Adj.Absent"
        << "Attendance\n";

    cout << string(95, '-') << "\n";

    //! Attendance calculation
    for (const auto &student : students)
    {
        int presentCount = 0;
        int absentCount = 0;
        int lateCount = 0;

        for (const auto &record : records)
        {
            if (record.studentID == student.id &&
                record.section == section)
            {
                if (record.status == "Present")
                    presentCount++;

                else if (record.status == "Absent")
                    absentCount++;

                else if (record.status == "Late")
                    lateCount++;
            }
        }

        //! Late attendance
        int effectivePresent =
            presentCount + (lateCount + 1) / 2;

        int effectiveAbsent =
            absentCount + lateCount / 2;

        //! attendance percentage
        double percentage =
            (100.0 * effectivePresent) / totalClasses;


        cout << left
            << setw(10) << student.id
            << setw(15) << student.name
            << setw(12) << presentCount
            << setw(12) << absentCount
            << setw(10) << lateCount
            << setw(14) << effectivePresent
            << setw(14) << effectiveAbsent
            << fixed << setprecision(1)
            << percentage << "%\n";

        //! students with low attendance
        if (percentage < 75.0)
        {
            warnings.push_back({student.id,
                                student.name,
                                percentage});
        }
    }

    //! LOW ATTENDANCE WARNING-----------------
    cout << "\n========== LOW ATTENDANCE WARNINGS ==========\n";

    if (warnings.empty())
    {
        cout << "All students meet the minimum attendance requirement!\n";
    }
    else
    {
        for (const auto &warning : warnings)
        {
            cout << "\nWARNING: "
                << warning.name
                << " (ID: " << warning.id << ")";

            cout << "\nAttendance: "
                << fixed << setprecision(1)
                << warning.percentage << "%";

            cout << "\nRequired: 75%\n";
        }
    }

    cout << "\n=============================================\n";
}


void Attendance::viewStudentAttendanceReport()
{
    FileManager fileManager;

    //! Load saved attendance records
    records = fileManager.loadAttendanceRecords();

    cout << "\n========== INDIVIDUAL STUDENT REPORT ==========\n";

    if (records.empty())
    {
        cout << "No attendance records found.\n";
        return;
    }

    string section;
    int studentID;

    cout << "Enter section: ";
    cin >> section;

    //! Validate Student ID
    while (true)
    {
        cout << "Enter Student ID: ";

        if (!(cin >> studentID))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cout << "Invalid ID! Enter a number.\n";
            continue;
        }

        if (studentID > 0)
            break;

        cout << "Student ID must be positive.\n";
    }

    string studentName;
    bool studentFound = false;

    //! Search student
    for (const auto& record : records)
    {
        if (record.studentID == studentID &&
            record.section == section)
        {
            studentName = record.studentName;
            studentFound = true;
            break;
        }
    }

    if (!studentFound)
    {
        cout << "\nNo attendance records found for this student.\n";
        return;
    }

    //!  dates for the student
    vector<string> dates;

    for (const auto& record : records)
    {
        if (record.section == section)
        {
            if (find(dates.begin(), dates.end(), record.date)
                == dates.end())
            {
                dates.push_back(record.date);
            }
        }
    }

    sort(dates.begin(), dates.end());

    int presentCount = 0;
    int absentCount = 0;
    int lateCount = 0;
    int noRecordCount = 0;

    cout << "\n========================================\n";
    cout << "         STUDENT ATTENDANCE REPORT\n";
    cout << "========================================\n";

    cout << "Student ID   : " << studentID << "\n";
    cout << "Student Name : " << studentName << "\n";
    cout << "Section      : " << section << "\n";

    cout << "\n---------- ATTENDANCE HISTORY ----------\n";

    cout << left
        << setw(18) << "Date"
        << "Status\n";

    cout << string(35, '-') << "\n";


    //! attendance status for each date
    for (const auto& date : dates)
    {
        string status = "No Record";

        for (const auto& record : records)
        {
            if (record.studentID == studentID &&
                record.section == section &&
                record.date == date)
            {
                status = record.status;
                break;
            }
        }

        if (status == "Present")
            presentCount++;

        else if (status == "Absent")
            absentCount++;

        else if (status == "Late")
            lateCount++;

        else
            noRecordCount++;

        cout << left
            << setw(18) << date
            << status << "\n";
    }

    //! total classes
    int totalClasses = dates.size();

    //! Late attendance rule
    int effectivePresent =
        presentCount + (lateCount + 1) / 2;

    int effectiveAbsent =
        absentCount + lateCount / 2;

    //! attendance percentage
    double percentage =
        (100.0 * effectivePresent) / totalClasses;

    cout << "\nAdjusted Present: " << effectivePresent;
    cout << "\nAdjusted Absent: " << effectiveAbsent;

    cout << "\n---------- ATTENDANCE STATISTICS ----------\n";

    cout << "Total Classes       : " << totalClasses << "\n";
    cout << "Present             : " << presentCount << "\n";
    cout << "Absent              : " << absentCount << "\n";
    cout << "Late                : " << lateCount << "\n";
    cout << "No Record           : " << noRecordCount << "\n";

    cout << "\nAdjusted Present    : " << effectivePresent << "\n";
    cout << "Adjusted Absent     : " << effectiveAbsent << "\n";

    cout << fixed << setprecision(1);

    cout << "\nAttendance Percentage: "
        << percentage << "%\n";

    //! Low attendance warning
    if (percentage < 75.0)
    {
        cout << "\nWARNING: Attendance is below 75%!\n";
    }
    else
    {
        cout << "\nStudent meets the minimum attendance requirement!\n";
    }

    cout << "\n===========================================\n";
}