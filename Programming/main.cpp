#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <cmath>
#include <limits>
#include "IntervalTree.h"
#include "patientInfo.h"
#include "priorityQueue.h"

// g++ main.cpp priorityQueue.cpp IntervalTree.cpp patientInfo.cpp -o main

int updateTime (int currentTime);
void pressEnter();
std::string generateScheduledFilename(std::string date);
std::string generateWalkInFilename(std::string date, int time);
int readScheduleFilename (std::string date, std::vector<Patient>& vec, std::map<std::string, IntervalTree>& doctorTrees);
std::vector<int> readWalkInFilename (std::string date, int time, std::vector<Patient>& vec, std::map<std::string, IntervalTree>& doctorTrees);

int main (int argc, char* argv[]) {
    
    std::string date = argv[1];
    std::vector<Patient> vec;

    // Create doctor trees (and a map to store them for easy access)
    std::map< std::string, IntervalTree > doctorTrees;
    doctorTrees["Jenny"] = IntervalTree();
    doctorTrees["Taylor"] = IntervalTree();
    doctorTrees["Paul"] = IntervalTree();

    int totalPatients = 0;
    int totalWalkIns = 0;
    int sentElsewhereWalkIns = 0;

    std::cout << "--------------------\nThe day is " << date << ".\n--------------------" << std::endl;

    // time is 7am (before walk-ins come)
    int time = 7;
    std::cout << "--------------------\nStart of day at 7:00am.\n--------------------" << std::endl;

    // this reads in patient information into a vector and adds their appointments to the doctors' interval trees
    // and counts the number of appointments that were just added in (also # of patients with appointments)
    int countAppointments = readScheduleFilename(date, vec, doctorTrees);
    totalPatients += countAppointments;
    
    std::cout << " * " << countAppointments << " appointments are scheduled for today." << std::endl;

    // output file for each doctor of their list of appointments for the day
    for (auto& [docName, docTree] : doctorTrees) {
        docTree.printAppointments(docName, date);
    }
    
    std::cout << " * Daily schedules have been generated for each doctor. They are available in the output folder." << std::endl;
    std::cout << std::endl;

    time = updateTime(time);
    
    // time is 8am through 7pm
    while (time != 20) { // while time is not 8pm, do hourly updates

        // user should press enter to continue with the hour
        pressEnter();

        if (time < 12) {
            std::cout << "--------------------\nNew hour: " << time << ":00am.\n--------------------" << std::endl;
        } else if (time == 12) {
            std::cout << "--------------------\nNew hour: " << time << ":00pm.\n--------------------" << std::endl;
        } else {
            std::cout << "--------------------\nNew hour: " << time-12 << ":00pm.\n--------------------" << std::endl;
        }

        // this adds to the vector the walk in patients that came in this hour
        // new appointments are added to doctorTrees until no more space is in the hour, the rest are sent elsewhere
        // {# of walk ins, # of walk ins who got appointments, # of walk ins sent elsewhere} is returned into hourCounts
        std::vector<int> hourCounts = readWalkInFilename(date, time, vec, doctorTrees);

        std::cout << " * " << hourCounts.at(0) << " patients walked in." << std::endl;
        std::cout << " * " << hourCounts.at(1) << " walk-in patients successfully got an appointment." << std::endl;
        std::cout << " * " << hourCounts.at(2) << " walk-in patients were turned away." << std::endl;

        totalPatients += hourCounts.at(0);
        totalWalkIns += hourCounts.at(0);
        sentElsewhereWalkIns += hourCounts.at(2);

        // output file for each doctor of their list of appointments for the day
        for (auto& [docName, docTree] : doctorTrees) {
            docTree.printAppointments(docName, date);
        }

        std::cout << " * Daily schedules have been updated for each doctor. They are available in the output folder." << std::endl;
        std::cout << std::endl;

        time = updateTime(time);
    }

    // time is 8pm
    std::cout << "--------------------\nEnd of day at 8:00pm.\n--------------------" << std::endl;
    
    // sort on first name then last name to have final result be sorted by last name, first name
    radixSortByFirstName(vec);
    radixSortByLastName(vec);

    // write the sorted patient names to an output file
    writeNamesToFile(vec, date);
    
    std::cout << " * " << totalPatients << " patient records written to day's patient records." << std::endl;
    std::cout << " * " << std::round( ((double)sentElsewhereWalkIns) * 100 / totalWalkIns) << "% of walk-ins were sent elsewhere." << std::endl;
    std::cout << "--------------------" << std::endl;

    return 0;
}

// Function to update time. Increments by 1 but is put into a function so the increment can change if needed
int updateTime (int currentTime) {
    return currentTime + 1;
}

// Function to require user to press enter, used before the new hour
void pressEnter() {
    // Prompt the user to press Enter to proceed
    std::cout << "Press the ENTER key to continue to the next hour: ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return;
}

// Function to create the filename for the input file of scheduled appointments and patients
std::string generateScheduledFilename(std::string date) {
    return "../Input/" + date + "_scheduled.txt";
}

// Function to create the filename for the input file of walk-in patients
std::string generateWalkInFilename(std::string date, int time) {
    return "../Input/" + date + "-" + std::to_string(time) + "_walk_ins.txt";
}

// Function to read in scheduled appointments and patients
// Reads in input file, adds to patient vector, and adds appointments to interval trees
// Returns statistic: # of patients with appointments
int readScheduleFilename (std::string date, std::vector<Patient>& vec, std::map<std::string, IntervalTree>& doctorTrees) {

    std::string filename = generateScheduledFilename(date);

    std::ifstream input;
    input.open(filename);
    if (input.fail()) {
        std::cerr << "Can't open file." << std::endl;
        return 1;
    }

    std::string line, lname, fname, gender, doctor, strStart;
    int start, end;
    int patientCount = 0; // keep count of patients added

    std::getline(input, line); // get header out
    while (std::getline(input, line)) {
        std::stringstream streamline(line);
        // parse elements of line
        // ex: Doe,John,Male,Jenny,0900
        getline(streamline, lname, ',');
        getline(streamline, fname, ',');
        getline(streamline, gender, ',');
        getline(streamline, doctor, ',');
        getline(streamline, strStart, ',');
        start = std::stoi(strStart);
        end = start + 29;

        // create patient node
        Patient currentPatient(fname, lname, gender, "Appointment");

        // add patient to linked list and vector
        vec.push_back(currentPatient);

        // add patient to interval tree for their doctor
        if (doctorTrees[doctor].insertInterval(lname, fname, start, end)) {
            //std::cout << "Appointment inserted." << std::endl;
        } else {
            //std::cout << "Failed to insert appointment." << std::endl;
        }

        // increment count
        patientCount++;
    }
    input.close();

    return patientCount;
}

// Function to read in walk-in patients
// Reads in input file, adds to patient vector, and adds to hour's priority queue
// Then adds walk-in appointments in if room
// Returns vector of statistics: {# of walk ins, # of walk ins who got appointments, # of walk ins sent elsewhere}
std::vector<int> readWalkInFilename (std::string date, int time, std::vector<Patient>& vec, std::map<std::string, IntervalTree>& doctorTrees) {

    std::string filename = generateWalkInFilename(date, time);

    std::ifstream input;
    input.open(filename);
    if (input.fail()) {
        std::cerr << "Can't open file." << std::endl;
        std::vector<int> emptyVector;
        return emptyVector;
    }

    std::string line, lname, fname, gender, strTriageLevel;
    int triageLevel;
    int patientCount = 0; // keep count of patients added
    PriorityQueue pq;

    std::getline(input, line); // get header out
    while (std::getline(input, line)) {
        std::stringstream streamline(line);
        // parse elements of line
        // ex: Medeiros,Darren,Male,5
        getline(streamline, lname, ',');
        getline(streamline, fname, ',');
        getline(streamline, gender, ',');
        getline(streamline, strTriageLevel, ',');
        triageLevel = std::stoi(strTriageLevel);

        // create patient node
        Patient currentPatient(fname, lname, gender, "Walk in");

        // add patient to linked list and vector
        vec.push_back(currentPatient);

        // add patients to priority queue
        pq.enqueue(lname, fname, gender, triageLevel);

        // increment count
        patientCount++;
    }
    input.close();

    // so far patients have been read in, added to patients vector, and added to priority queue
    // now, iterate through priority queue until either empty or no more spots in any doctor's trees
    int walkInsAdded = 0;

    bool spotsLeftInHour = true;
    while ( (spotsLeftInHour == true) && (pq.get_size() != 0) ) {

        PQNode* currentPatient = pq.top();

        if (currentPatient->get_gender() == "Male") {
            
            if (doctorTrees["Paul"].insertInterval(currentPatient->get_lastName(), currentPatient->get_firstName(), (time*100), (time*100)+14)) {
                pq.dequeue();
                walkInsAdded++;
                //std::cout << "Appointment inserted." << std::endl;
            } else if (doctorTrees["Paul"].insertInterval(currentPatient->get_lastName(), currentPatient->get_firstName(), (time*100)+15, (time*100)+29)) {
                pq.dequeue();
                walkInsAdded++;
                //std::cout << "Appointment inserted." << std::endl;
            } else if (doctorTrees["Paul"].insertInterval(currentPatient->get_lastName(), currentPatient->get_firstName(), (time*100)+30, (time*100)+44)) {
                pq.dequeue();
                walkInsAdded++;
                //std::cout << "Appointment inserted." << std::endl;
            } else if (doctorTrees["Paul"].insertInterval(currentPatient->get_lastName(), currentPatient->get_firstName(), (time*100)+45, (time*100)+59)) {
                pq.dequeue();
                walkInsAdded++;
                //std::cout << "Appointment inserted." << std::endl;
            } else if (doctorTrees["Taylor"].insertInterval(currentPatient->get_lastName(), currentPatient->get_firstName(), (time*100), (time*100)+14)) {
                pq.dequeue();
                walkInsAdded++;
                //std::cout << "Appointment inserted." << std::endl;
            } else if (doctorTrees["Taylor"].insertInterval(currentPatient->get_lastName(), currentPatient->get_firstName(), (time*100)+15, (time*100)+29)) {
                pq.dequeue();
                walkInsAdded++;
                //std::cout << "Appointment inserted." << std::endl;
            } else if (doctorTrees["Taylor"].insertInterval(currentPatient->get_lastName(), currentPatient->get_firstName(), (time*100)+30, (time*100)+44)) {
                pq.dequeue();
                walkInsAdded++;
                //std::cout << "Appointment inserted." << std::endl;
            } else if (doctorTrees["Taylor"].insertInterval(currentPatient->get_lastName(), currentPatient->get_firstName(), (time*100)+45, (time*100)+59)) {
                pq.dequeue();
                walkInsAdded++;
                //std::cout << "Appointment inserted." << std::endl;
            } else if (doctorTrees["Jenny"].insertInterval(currentPatient->get_lastName(), currentPatient->get_firstName(), (time*100), (time*100)+14)) {
                pq.dequeue();
                walkInsAdded++;
                //std::cout << "Appointment inserted." << std::endl;
            } else if (doctorTrees["Jenny"].insertInterval(currentPatient->get_lastName(), currentPatient->get_firstName(), (time*100)+15, (time*100)+29)) {
                pq.dequeue();
                walkInsAdded++;
                //std::cout << "Appointment inserted." << std::endl;
            } else if (doctorTrees["Jenny"].insertInterval(currentPatient->get_lastName(), currentPatient->get_firstName(), (time*100)+30, (time*100)+44)) {
                pq.dequeue();
                walkInsAdded++;
                //std::cout << "Appointment inserted." << std::endl;
            } else if (doctorTrees["Jenny"].insertInterval(currentPatient->get_lastName(), currentPatient->get_firstName(), (time*100)+45, (time*100)+59)) {
                pq.dequeue();
                walkInsAdded++;
                //std::cout << "Appointment inserted." << std::endl;
            } else {
                //std::cout << "Failed to insert appointment." << std::endl;
                spotsLeftInHour = false;
            }

        // Now if preferred gender is Female
        } else {
            
            if (doctorTrees["Jenny"].insertInterval(currentPatient->get_lastName(), currentPatient->get_firstName(), (time*100), (time*100)+14)) {
                pq.dequeue();
                walkInsAdded++;
                //std::cout << "Appointment inserted." << std::endl;
            } else if (doctorTrees["Jenny"].insertInterval(currentPatient->get_lastName(), currentPatient->get_firstName(), (time*100)+15, (time*100)+29)) {
                pq.dequeue();
                walkInsAdded++;
                //std::cout << "Appointment inserted." << std::endl;
            } else if (doctorTrees["Jenny"].insertInterval(currentPatient->get_lastName(), currentPatient->get_firstName(), (time*100)+30, (time*100)+44)) {
                pq.dequeue();
                walkInsAdded++;
                //std::cout << "Appointment inserted." << std::endl;
            } else if (doctorTrees["Jenny"].insertInterval(currentPatient->get_lastName(), currentPatient->get_firstName(), (time*100)+45, (time*100)+59)) {
                pq.dequeue();
                walkInsAdded++;
                //std::cout << "Appointment inserted." << std::endl;
            } else if (doctorTrees["Taylor"].insertInterval(currentPatient->get_lastName(), currentPatient->get_firstName(), (time*100), (time*100)+14)) {
                pq.dequeue();
                walkInsAdded++;
                //std::cout << "Appointment inserted." << std::endl;
            } else if (doctorTrees["Taylor"].insertInterval(currentPatient->get_lastName(), currentPatient->get_firstName(), (time*100)+15, (time*100)+29)) {
                pq.dequeue();
                walkInsAdded++;
                //std::cout << "Appointment inserted." << std::endl;
            } else if (doctorTrees["Taylor"].insertInterval(currentPatient->get_lastName(), currentPatient->get_firstName(), (time*100)+30, (time*100)+44)) {
                pq.dequeue();
                walkInsAdded++;
                //std::cout << "Appointment inserted." << std::endl;
            } else if (doctorTrees["Taylor"].insertInterval(currentPatient->get_lastName(), currentPatient->get_firstName(), (time*100)+45, (time*100)+59)) {
                pq.dequeue();
                walkInsAdded++;
                //std::cout << "Appointment inserted." << std::endl;
            } else if (doctorTrees["Paul"].insertInterval(currentPatient->get_lastName(), currentPatient->get_firstName(), (time*100), (time*100)+14)) {
                pq.dequeue();
                walkInsAdded++;
                //std::cout << "Appointment inserted." << std::endl;
            } else if (doctorTrees["Paul"].insertInterval(currentPatient->get_lastName(), currentPatient->get_firstName(), (time*100)+15, (time*100)+29)) {
                pq.dequeue();
                walkInsAdded++;
                //std::cout << "Appointment inserted." << std::endl;
            } else if (doctorTrees["Paul"].insertInterval(currentPatient->get_lastName(), currentPatient->get_firstName(), (time*100)+30, (time*100)+44)) {
                pq.dequeue();
                walkInsAdded++;
                //std::cout << "Appointment inserted." << std::endl;
            } else if (doctorTrees["Paul"].insertInterval(currentPatient->get_lastName(), currentPatient->get_firstName(), (time*100)+45, (time*100)+59)) {
                pq.dequeue();
                walkInsAdded++;
                //std::cout << "Appointment inserted." << std::endl;
            } else {
                //std::cout << "Failed to insert appointment." << std::endl;
                spotsLeftInHour = false;
            }

        }

    }

    // now all walk in appointments that can be added are added in
    // so clear out the priority queue getting a count of the walk ins that were not able to be added and were sent elsewhere
    int walkInsSentElsewhere = 0;
    while (pq.get_size() != 0) {
        walkInsSentElsewhere++;
        pq.dequeue();
    }

    // put all the counts in a vector to return
    // {walkIns, added, sentElsewhere}
    std::vector<int> counts;
    counts.push_back(patientCount);
    counts.push_back(walkInsAdded);
    counts.push_back(walkInsSentElsewhere);

    return counts;
}

