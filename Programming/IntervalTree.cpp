#include <fstream>
#include <string>
#include <iomanip>
#include "IntervalTree.h"

// Function to insert a new interval node, or appointment, into the interval tree
// If able to insert an appointment, return true, otherwise return false
bool IntervalTree::insertInterval(const std::string& lastName, const std::string& firstName, int start, int end) {
    // Check for overlaps
    if (hasOverlap(start, end)) {
        return false; // Cannot insert because of overlap
    }

    // Insert the interval if no overlap
    intervals.push_back(Appointment(lastName, firstName, start, end));
    return true;
}

// Method to check if an interval overlaps with existing intervals
bool IntervalTree::hasOverlap(int start, int end) {
    for (const auto& interval : intervals) {
        // Check if the new interval overlaps with an existing interval
        if (start < interval.endTime && end > interval.startTime) {
            return true; // Overlap detected
        }
    }
    return false; // No overlap
}

// Function to print appointments out from an interval tree
void IntervalTree::printAppointments(const std::string& doctorName, const std::string& date) const {
    std::string fileName = "../Output/" + date + "_appointments_" + doctorName + ".txt";
    std::ofstream outFile(fileName);

    if (!outFile.is_open()) {
        std::cerr << "Failed to open file: " << fileName << std::endl;
        return;
    }

    outFile << "Appointments for Dr. " << doctorName << ":\n";

    std::vector<Appointment> sortedIntervals = intervals;
    std::sort(sortedIntervals.begin(), sortedIntervals.end(), [](const Appointment& a, const Appointment& b) {
                  return a.startTime < b.startTime;
              });

    for (const auto& interval : sortedIntervals) {
        // Had to slave away to do this one !!!
        // Get hours and minutes
        int startHours = interval.startTime / 100;
        int startMinutes = interval.startTime % 100;

        // Get hours and minutes in end
        int endHours = interval.endTime / 100;
        int endMinutes = interval.endTime % 100;

        // output to file that is nice adn good !
        outFile << interval.firstName << " (" << interval.lastName << "): Start time: "
                << std::setw(2) << std::setfill('0') << startHours << ':'
                << std::setw(2) << std::setfill('0') << startMinutes
                << ", End time: "
                << std::setw(2) << std::setfill('0') << endHours << ':'
                << std::setw(2) << std::setfill('0') << endMinutes << "\n";
    }

    outFile.close();
}

