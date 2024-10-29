#include <iostream>
#include <string>
#include "priorityQueue.h"

// **************************
// PQNode definitions
// **************************

// Default constructor
PQNode::PQNode(){
    this->lastName = "";
    this->firstName = "";
    this->gender = "";
    this->triage_level = 0;
    this->next = nullptr;
}

// Parameterized constructor w/o PQNode* next argument
PQNode::PQNode(std::string lastName, std::string firstName, std::string gender, int triage_level)
{
    this->lastName = lastName;
    this->firstName = firstName;
    this->gender = gender;
    this->triage_level = triage_level;
    this->next = nullptr;
}

// Parameterized constructor w/ PQNode* next argument
PQNode::PQNode(std::string lastName, std::string firstName, std::string gender, int triage_level, PQNode* next){
    this->lastName = lastName;
    this->firstName = firstName;
    this->gender = gender;
    this->triage_level = triage_level;
    this->next = next;
}

// Destructor
PQNode::~PQNode()
{
    if(this->next != nullptr)
    {
        delete this->next;
    }
}

// Getter for lastName
std::string PQNode::get_lastName() {
    return this->lastName;
}

// Getter for firstName
std::string PQNode::get_firstName() {
    return this->firstName;
}

// Getter for gender
std::string PQNode::get_gender() {
    return this->gender;
}

// **************************
// PriorityQueue definitions
// **************************

// Default constructor
PriorityQueue::PriorityQueue(){
    this->head = nullptr;
    this->size = 0;
}

// Parameterized constructor
PriorityQueue::PriorityQueue(std::string lastName, std::string firstName,std::string gender){
    this->head = new PQNode(lastName, firstName, gender, 0);
    this->size = 1;
}

// Destructor
PriorityQueue::~PriorityQueue() {
    delete this->head;
    this->head = nullptr;
}

// Enqueue function to add a new patient into the priority queue
void PriorityQueue::enqueue(std::string lastName, std::string firstName, std::string gender, int triage_level){

    // if nothing in list, add
    if (this->head == nullptr) {
        this->head = new PQNode(lastName, firstName,gender, triage_level);
        this->size++;
    }
    // if things in list
    else {
        if (triage_level < (this->head)->triage_level) { // if higher priority than 1st element
            PQNode* newNode = new PQNode (lastName, firstName, gender, triage_level, this->head);
            this->head = newNode;
            this->size++;
            return;
        }
        // if higher priority than rest of the elements
        PQNode* currentNode = this->head;
        while (currentNode->next != nullptr) {
            // runs until we are at the tail
            if (triage_level < (currentNode->next)->triage_level) {
                currentNode->next = new PQNode(lastName, firstName, gender, triage_level, currentNode->next);
                this->size++;
                return;
            }
            currentNode = currentNode->next; // move over to next node if condition not met
        }

        // add to end (was not higher priority than anything before the tail)
        // currentNode is now the tail after leaving the while loop
        currentNode->next = new PQNode(lastName, firstName, gender, triage_level); // points to nullptr
        this->size++;
    }
}

// Dequeue function to remove the first element from the priority queue
void PriorityQueue::dequeue(){

    // if nothing in here just return
    if (this->size == 0) {
        return;
    }

    // move head to the head node's next and update size
    this->head = (this->head)->next;
    this->size--;
}

// Top function to access the patient next in queue
PQNode* PriorityQueue::top(){
    return this->head;
}

// Getter function to get the size of the priority queue
int PriorityQueue::get_size(){
    return this->size;
}

