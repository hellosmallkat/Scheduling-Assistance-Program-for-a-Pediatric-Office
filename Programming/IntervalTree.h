#ifndef INTERVALTREE_H
#define INTERVALTREE_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

struct Appointment  {
    std::string lastName;
    std::string firstName;
    int startTime;
    int endTime;
    
    // Constructor
    Appointment(const std::string& lastName, const std::string& firstName, int start, int end)
        : lastName(lastName), firstName(firstName), startTime(start), endTime(end) {}
};

class IntervalTree {
    private:
        std::vector<Appointment> intervals;

    public:
        IntervalTree() {}
        ~IntervalTree() {}

        bool insertInterval(const std::string& lastName, const std::string& firstName, int start, int end);
        bool hasOverlap(int start, int end);
        void printAppointments(const std::string& doctorName, const std::string& date) const;
        void printIntervals() const;
};

#endif
