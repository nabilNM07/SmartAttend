#ifndef STUDENT_H
#define STUDENT_H

#include <string>
#include <vector>
using namespace std;

class Student
{
    private:
        int id;
        string name;
        string department;
        string section;
    
    public:
        Student();

        Student(int id, string name, string department, string section);

        int getId() const;
        string getName() const;
        string getDepartment() const;
        string getSection() const;
};

class StudentManager
{
    public:
        void showStudentMenu();

    private:
        void addStudentsBySection();
        void displayStudentsBySection();
        void searchStudent();
        void updateStudent();
        void deleteStudent();
};

#endif 
