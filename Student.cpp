#include "Student.h"
#include "FileManager.h"

#include <iostream>
#include <iomanip>
#include <limits>
#include <algorithm>

using namespace std;



//! student class
Student::Student()
{
    id = 0;
    name = "";
    department = "";
    section = "";
}

Student::Student(int id, string name, string department, string section)
{
    this->id = id;
    this->name = name;
    this->department = department;
    this->section = section;
}

//! getter functions
int Student::getId() const
{
    return id;
}

string Student::getName() const
{
    return name;
}

string Student::getDepartment() const
{
    return department;
}

string Student::getSection() const
{
    return section;
}



//! STudent management menu
void StudentManager::showStudentMenu()
{
    int choice;

    while (true)
    {
        cout << "\n========== STUDENT MANAGEMENT ==========\n";

        cout << "1. Add Students\n";
        cout << "2. Display Students\n";
        cout << "3. Search Student\n";
        cout << "4. Update Student\n";
        cout << "5. Delete Student\n";
        cout << "6. Back to Main Menu\n";

        cout << "\nEnter your choice: ";

        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cout << "Invalid input! Enter a number.\n";
            continue;
        }

        switch (choice)
        {
            case 1:
                addStudentsBySection();
                break;

            case 2:
                displayStudentsBySection();
                break;

            case 3:
                searchStudent();
                break;

            case 4:
                updateStudent();
                break;

            case 5:
                deleteStudent();
                break;

            case 6:
                return;

            default:
                cout << "Invalid choice! Select 1-6.\n";
        }
    }
}


// ========================================
// 1. ADD STUDENTS SECTION-WISE
// ========================================

void StudentManager::addStudentsBySection()
{
    FileManager fileManager;

    vector<Student> students = fileManager.loadStudents();

    string section;

    cout << "\n========== REGISTER STUDENTS ==========\n";

    cout << "Enter Section: ";
    cin >> section;

    int addedCount = 0;

    while (true)
    {
        int id;

        cout << "\nEnter Student ID: ";

        while (!(cin >> id) || id <= 0)
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cout << "Invalid ID! ID should be positive only!\n ";
        }


        //! Chicking duplicate ID
        bool duplicate = false;

        for (const auto& student : students)
        {
            if (student.getId() == id)
            {
                duplicate = true;
                break;
            }
        }

        if (duplicate)
        {
            cout << "This Student ID already exists!\n";
            continue;
        }


        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        string name, department;

        cout << "Enter Student Name: ";
        getline(cin, name);

        cout << "Enter Department: ";
        getline(cin, department);


        //! checking input validity
        if (name.empty() || department.empty() ||
            name.find('|') != string::npos ||
            department.find('|') != string::npos)
        {
            cout << "Invalid student information. Try again.\n";
            continue;
        }


        //! Create student object
        Student newStudent(id, name, department, section);

        students.push_back(newStudent);

        addedCount++;

        cout << "\nStudent added successfully!\n";

        cout << "\nAdd another student to section "<< section << "? (Y/N): ";

        char choice;
        cin >> choice;

        if (choice != 'Y' && choice != 'y')
            break;
    }


    //! Saving students to file
    if (addedCount > 0)
    {
        if (fileManager.saveStudents(students))
        {
            cout << "\n" << addedCount<< " student saved successfully!\n";
        }
        else
        {
            cout << "\nError saving student information!\n";
        }
    }
}



//! (2) DISPLAY STUDENTS SECTION-WISE
void StudentManager::displayStudentsBySection()
{
    FileManager fileManager;

    vector<Student> students = fileManager.loadStudents();

    if (students.empty())
    {
        cout << "\nNo students registered yet.\n";
        return;
    }

    string section;

    cout << "\nEnter Section: ";
    cin >> section;

    cout << "\n========== STUDENT LIST: "<< section << " ==========\n\n";

    cout << left
        << setw(12) << "ID"
        << setw(22) << "Name"
        << setw(15) << "Department"
        << "Section\n";

    cout << string(60, '-') << "\n";

    int count = 0;

    for (const auto& student : students)
    {
        if (student.getSection() == section)
        {
            cout << left
                << setw(12) << student.getId()
                << setw(22) << student.getName()
                << setw(15) << student.getDepartment()
                << student.getSection() << "\n";

            count++;
        }
    }

    if (count == 0)
        cout << "No students found in this section.\n";
    else
        cout << "\nTotal Students: " << count << "\n";
}



//! (3) SEARCH STUDENT BY ID
void StudentManager::searchStudent()
{
    FileManager fileManager;

    vector<Student> students = fileManager.loadStudents();

    int id;

    cout << "\nEnter Student ID to search: ";

    if (!(cin >> id))
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Invalid Student ID!\n";
        return;
    }

    for (const auto& student : students)
    {
        if (student.getId() == id)
        {
            cout << "\nStudent Found!\n";

            cout << "ID         : " << student.getId() << "\n";
            cout << "Name       : " << student.getName() << "\n";
            cout << "Department : " << student.getDepartment() << "\n";
            cout << "Section    : " << student.getSection() << "\n";

            return;
        }
    }

    cout << "\nStudent not found!\n";
}



//! (4) UPDATE STUDENT INFORMATION
void StudentManager::updateStudent()
{
    FileManager fileManager;

    vector<Student> students = fileManager.loadStudents();

    int id;

    cout << "\nEnter Student ID to update: ";

    if (!(cin >> id))
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Invalid Student ID!\n";
        return;
    }

    for (auto& student : students)
    {
        if (student.getId() == id)
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            string name, department, section;

            cout << "\nCurrent Student Found!\n";

            cout << "New Name: ";
            getline(cin, name);

            cout << "New Department: ";
            getline(cin, department);

            cout << "New Section: ";
            getline(cin, section);

            if (name.empty() || department.empty() ||
                section.empty() ||
                name.find('|') != string::npos ||
                department.find('|') != string::npos ||
                section.find('|') != string::npos)
            {
                cout << "Invalid information! Update cancelled.\n";
                return;
            }

            student = Student(id, name, department, section);

            if (fileManager.saveStudents(students))
                cout << "\nStudent updated successfully!\n";
            else
                cout << "\nError saving updated information!\n";

            return;
        }
    }

    cout << "\nStudent not found!\n";
}



//! (5) DELETE STUDENT
void StudentManager::deleteStudent()
{
    FileManager fileManager;

    vector<Student> students = fileManager.loadStudents();

    int id;

    cout << "\nEnter Student ID to delete: ";

    if (!(cin >> id))
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Invalid Student ID!\n";
        return;
    }

    auto it = find_if(
        students.begin(),
        students.end(),
        [id](const Student& student)
        {
            return student.getId() == id;
        }
    );

    if (it == students.end())
    {
        cout << "\nStudent not found!\n";
        return;
    }

    char confirmation;

    cout << "\nStudent: " << it->getName();
    cout << "\nAre you sure you want to delete this student? (Y/N): ";

    cin >> confirmation;

    if (confirmation != 'Y' && confirmation != 'y')
    {
        cout << "\nDeletion cancelled.\n";
        return;
    }

    students.erase(it);

    if (fileManager.saveStudents(students))
        cout << "\nStudent deleted successfully!\n";
    else
        cout << "\nError deleting student!\n";
}