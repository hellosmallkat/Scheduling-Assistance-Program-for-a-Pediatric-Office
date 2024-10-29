#include <fstream>
#include <string>
#include <iostream>
#include "patientInfo.h"

// Constructors
Patient::Patient(const std::string& firstName, const std::string& lastName,
            const std::string& gender, const std::string& type, Patient* next)
    : firstName(firstName), lastName(lastName), gender(gender), type(type), next(next) {}

Patient::Patient(const std::string& firstName, const std::string& lastName,
            const std::string& gender, const std::string& type)
    : firstName(firstName), lastName(lastName), gender(gender), type(type), next(nullptr) {}

// Function to write sorted objects to an output file
void writeNamesToFile(const std::vector<Patient>& arr, const std::string& date) {
    std::string filename = "../Output/" + date + "_sorted_patients.txt";
    
    std::ofstream outputFile(filename);
    if (!outputFile.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        return;
    }

    for (const Patient& p : arr) {
        outputFile << p.lastName << ", " << p.firstName << " (" << p.type << ")" << std::endl;
    }

    outputFile.close();
}

// Radix sort implementation for sorting by a specific digit (most significant digit)
// This performs bucket sort on each digit (for first name)
void radixSortByDigit(std::vector<Patient>& arr, int digit) {
    const int NUM_BUCKETS = 256; // Assuming ASCII characters

    // Create buckets
    std::vector<std::vector<Patient>> buckets(NUM_BUCKETS);

    // Distribute elements into buckets based on the digit
    for (const Patient& p : arr) {
        int bucketIndex = (digit < p.firstName.size()) ? static_cast<unsigned char>(p.firstName[digit]) : 0;
        buckets[bucketIndex].push_back(p);
    }

    // Collect elements back into the array
    int index = 0;
    for (auto& bucket : buckets) {
        for (const Patient& p : bucket) {
            arr[index++] = p;
        }
    }
}

// Radix sort implementation for sorting by a specific digit (most significant digit)
// This performs bucket sort on each digit (for last name)
void radixSortByDigit2(std::vector<Patient>& arr, int digit) {
    const int NUM_BUCKETS = 256; // Assuming ASCII characters

    // Create buckets
    std::vector<std::vector<Patient>> buckets(NUM_BUCKETS);

    // Distribute elements into buckets based on the digit
    for (const Patient& p : arr) {
        int bucketIndex = (digit < p.lastName.size()) ? static_cast<unsigned char>(p.lastName[digit]) : 0;
        buckets[bucketIndex].push_back(p);
    }

    // Collect elements back into the array
    int index = 0;
    for (auto& bucket : buckets) {
        for (const Patient& p : bucket) {
            arr[index++] = p;
        }
    }
}

// Radix sort by first name
void radixSortByFirstName(std::vector<Patient>& arr) {
    // Find the maximum length of first names
    int maxLength = 0;
    for (const Patient& p : arr) {
        maxLength = std::max(maxLength, static_cast<int>(p.firstName.size()));
    }

    // Perform radix sort for each digit in the first name (starting from the most significant digit)
    for (int digit = maxLength - 1; digit >= 0; --digit) {
        radixSortByDigit(arr, digit);
    }
}

// Radix sort by last name
void radixSortByLastName(std::vector<Patient>& arr) {
    // Find the maximum length of first names
    int maxLength = 0;
    for (const Patient& p : arr) {
        maxLength = std::max(maxLength, static_cast<int>(p.lastName.size()));
    }

    // Perform radix sort for each digit in the first name (starting from the most significant digit)
    for (int digit = maxLength - 1; digit >= 0; --digit) {
        radixSortByDigit2(arr, digit);
    }
}

