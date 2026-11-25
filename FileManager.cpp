#include "FileManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

//! Save attendance records to a file
bool FileManager::saveAttendanceRecords(
    const vector<AttendanceRecord>& records)
{
    ofstream file("data/attendance.txt");

    if(!file.is_open())
    {
        cout << "Error opening attendance file!\n";
        return false;
    }

    for(const auto& record : records)
    {
        file << record.section << "|"
            << record.date << "|" 
            << record.studentID << "|"
            << record.studentName << "|"
            << record.status << "\n";
    }


file.close();
return !file.fail();
}

//! Load attendance records from a file
vector<AttendanceRecord> FileManager::loadAttendanceRecords()
{
    vector<AttendanceRecord> records;

    ifstream file("data/attendance.txt");

    if(!file.is_open())
    {
        return records; 
    }

    string line;

    while(getline(file, line))
    {
        stringstream ss(line);

        AttendanceRecord record;
        string idText;
        if(!getline(ss, record.section, '|') ||
            !getline(ss, record.date, '|') ||
            !getline(ss, idText, '|') ||
            !getline(ss, record.studentName, '|') ||
            !getline(ss, record.status))
        {
            continue; 
        }
        stringstream idStream(idText);

        if(!(idStream >> record.studentID))
        {
            continue; 
        }

        records.push_back(record);
    }

    file.close();
    return records;
}



//! save student information to a file
bool FileManager::saveStudents(const vector<Student>& students)
{
    ofstream file("data/students.txt");

    if (!file.is_open())
    {
        cout << "Error opening students file!\n";
        return false;
    }

    for (const auto& student : students)
    {
        file << student.getId() << "|"
            << student.getName() << "|"
            << student.getDepartment() << "|"
            << student.getSection() << "\n";
    }

    file.close();

    return !file.fail();
}



//! Load student information from a file
vector<Student> FileManager::loadStudents()
{
    vector<Student> students;

    ifstream file("data/students.txt");

    if (!file.is_open())
        return students;

    string line;

    while (getline(file, line))
    {
        stringstream ss(line);

        string idText;
        string name;
        string department;
        string section;

        if (!getline(ss, idText, '|') ||
            !getline(ss, name, '|') ||
            !getline(ss, department, '|') ||
            !getline(ss, section))
        {
            continue;
        }

        int id;

        stringstream idStream(idText);

        if (!(idStream >> id))
            continue;

        students.emplace_back(
            id, name, department, section
        );
    }

    file.close();

    return students;
}