#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <stdlib.h>
#pragma once
#include "customer.h"
#include "vehicle.h"
#include "records.h"
#define borders "------------------------------------------------------------\n"
#define vinSize 17

//USED TO STORE ALL THE ANCILLARY FUNCTIONS FOR THE MAIN OPERATIONS

//Vector Regsitries
inline vector<Vehicle> vehicleReg;
inline vector<Customer> custReg;
inline vector<Records> recordList;

string getDate(); //Input and validate date string from user
Vehicle& searchVehicle(string plateNo); //return vehicle reference from plate number
//check for any conflicting records for specified vehicle within input date range
bool findConflict(string licensePlate, string startDate, string endDate);
//overloaded function to additionally check and disregard any conflicting records with the same customer (for modification)
bool findConflict(string licensePlate, string startDate, string endDate, string dlCheck);
double calculateFare(int vehicleFare, string startDate, string endDate); //calculate number of days and subsequent charge
string todayDate(); //get today's date in string format



