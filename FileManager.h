#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "Attendance.h"
#include "Student.h"

#include <vector>
using namespace std;

class FileManager 
{
    public:
    //! attendance
        bool saveAttendanceRecords(const vector<AttendanceRecord>& records);
        vector<AttendanceRecord> loadAttendanceRecords();

    //! student
        bool saveStudents(const vector<Student>& students);
        vector<Student> loadStudents();
};

#endif
