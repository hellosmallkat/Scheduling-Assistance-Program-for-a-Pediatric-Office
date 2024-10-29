#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

#include <vector>
#include <string>
#include "patientInfo.h"

class PQNode{
    private:
        std::string lastName;
        std::string firstName;
        std::string gender;
        int triage_level;
        PQNode* next;

        friend class PriorityQueue;
    public:
        PQNode();
        // Overloaded constructors
        PQNode(std::string lastName, std::string firstName, std::string gender, int triage_level);
        PQNode(std::string lastName, std::string firstName, std::string gender, int triage_level, PQNode* next);
        ~PQNode();
        
        std::string get_lastName();
        std::string get_firstName();
        std::string get_gender();
};

class PriorityQueue{
    private:
        PQNode* head;
        unsigned int size;

    public:
        PriorityQueue();
        PriorityQueue(std::string lastName, std::string firstName, std::string gender);
        ~PriorityQueue();

        void enqueue(std::string lastName, std::string firstName, std::string gender, int triage_level);
        void dequeue();
        PQNode* top();
        int get_size();
};

#endif
