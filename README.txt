# README: Scheduling Assistance Program for a Pediatric Office

## Brief Overview of Microcosm

Our program is to be used at a pediatric office which we assume to run from 8am to 
8pm and have three doctors: Jenny, Taylor, and Paul. We assume this office accepts 
walk-ins as patients might need to see a doctor if they are sick despite not having 
an appointment scheduled already.


## Our Data Structures and Algorithms

Our program uses interval trees, priority queues, radix sort, and bucket sort. Interval 
trees are used to add in new appointments while preventing any overlapping appointments 
from being made. Each doctor has their own interval tree. Priority queues are used to 
determine which walk-ins need to see a doctor most urgently and to allocate any available 
appointment slots to them first. A new priority queue is made each hour. Radix sort and 
bucket sort are used to sort the list of patients as they are output, with radix sort 
using bucket sort as its helper sorting algorithm. Using all of these structures and 
algorithms this program is effective and accurate.


## Functionality

Our program is meant to assist in scheduling at a pediatric office. It reads in input 
files of patient information. If the patient already has an appointment, they are added 
in at the start of the day. Walk-ins are read in at the start of each hour and get an 
appointment if there is an available doctor during that hour. Our program stores basic 
patient information and each doctor's schedule. It outputs each doctor's schedule and 
summary statistics at each hour of the day. At the end of the day, it prints out a 
sorted list of all patients. 


## Usage Instructions

Before running the program, an input file must be added by the program user to reflect 
the patients that have appointments that day. This start of day input file should be 
named “YYYY-MM-DD_scheduled.txt”. Once this file is completed, the program is ran by 
navigating to the Programming folder and running “./main YYYY-MM-DD” in the terminal. 
Before the program attempts to read in files for the new hour, the user must press enter 
on their keyboard when prompted. Before pressing enter, the user should complete the 
input file for that hour, making sure it is saved and correctly formatted. These hourly 
files for walk-ins are required from 8am to 7pm. They should be named 
“YYYY-MM-DD-HH_walk_ins.txt”, where if the time is 7am, the hour, HH, is just 7, and 
if the time is 4pm, the hour, HH, is 16 for military time. After 7pm passes, the program 
will complete the actions for that hour and finally the end of day actions, as already 
described above. 

There are example input files for two days in the input folder. Following these examples 
will lead to success. 

To run the program, first make sure the terminal is in the Programming folder using "cd 
Programming". Then, compile using "g++ main.cpp priorityQueue.cpp IntervalTree.cpp 
patientInfo.cpp -o main". Run the executable using "./main YYYY-MM-DD", where YYYY-MM-DD 
is the desired date in the described format. For the two example days that there are 
already input files for in the workspace, use "./main 2024-04-15" and "./main 2024-04-16". 

As described above, there is a required command line argument which is the date in 
YYYY-MM-DD format. There should also be the scheduled appointment text file created before 
beginning the program. The walk-in files can be added as the program is running as there 
are stops in the program which require the user to press enter before going onto the next 
hour. Make sure the input file for the next hour is created and properly named before 
pressing enter. 

Please note that the terminal commands described above should not include the quotations 
and should just be the content within the quotations. 


## Dependencies

The program uses the C++ programming language. Here is the list of libraries used: 
iostream, fstream, sstream, string, map, vector, algorithm, cmath, limits, iomanip. 
We define our own classes and structures for priority queue, interval trees, and 
patients. 


## Installation Instructions

To run our program locally, please download all files into a .zip and move it into a 
directory where you want them to be located. Unzip the files. Make sure all dependencies 
are met. Then, open a terminal and follow the instructions under the "Usage Instructions" 
section to run the program yourself. 


