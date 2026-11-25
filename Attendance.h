#ifndef ATTENDANCE_H
#define ATTENDANCE_H

#include <string>
#include <vector>
using namespace std;

struct AttendanceRecord {
    int studentID;
    string studentName;
    string section;
    string date;
    string status; 
};

class Attendance {
    private:
        vector<AttendanceRecord> records;
    
    public:
        void takeAttendance();
        void viewAttendanceRecords();
        void viewStudentAttendanceReport();
};

#endif