#ifndef PATIENT_INFO_H
#define PATIENT_INFO_H

#include <string>
#include <vector>

// Define the Patient structure
struct Patient {
    std::string firstName;
    std::string lastName;
    std::string gender;
    std::string type;
    Patient* next;

    // Constructors
    Patient(const std::string& firstName, const std::string& lastName,
            const std::string& gender, const std::string& type, Patient* next);
    Patient(const std::string& firstName, const std::string& lastName,
            const std::string& gender, const std::string& type);
};

// Function to write sorted objects to an output file
void writeNamesToFile(const std::vector<Patient>& arr, const std::string& filename);

// Radix sort implementation for sorting by a specific digit (most significant digit)
// These use bucket sort
void radixSortByDigit(std::vector<Patient>& arr, int digit);
void radixSortByDigit2(std::vector<Patient>& arr, int digit);

// Radix sort by first name
void radixSortByFirstName(std::vector<Patient>& arr);

// Radix sort by last name
void radixSortByLastName(std::vector<Patient>& arr);

#endif // PATIENT_INFO_H
