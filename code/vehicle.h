#pragma once
#include <iostream>
#include<string>
#include <chrono>
#include <ctime>  //using C++ 17 libraries to stay in line with replit

//Vehicle column headers for console output
#define vehicleCols std::string()  + "Plate\t" + "Make\t" + "Model\t" + "Category\t" + "YOM\t" + "Odometer(KM)\t" + "DailyRate(AED)\t" + "MaintenanceLog\n"


using namespace std;


struct vehicleDetails{  //TSV RECORDS FOR VEHICLES MUST FOLLOW THE FORMAT: VIN, License, Make, Model, Category, YOM, ODO, Daily Rate (AED), Log
            std::string licenseNum; //License Plate number
            std::string make; //e.g. Nissan, Toyota, Norththrop Grumman
            std::string model; //e.g. Sunny, LLV, Sierra
            std::string category; //e.g Van, SUV, Sedan, APC
            std::string log;  //check for roadworthiness
            int yom; //year of manufacture
            int odoValue;  //odometer value
            int dailyRate; //Daily Rate should be whole AED
            
            vehicleDetails()
            {
                licenseNum = "N/A";
                make = "N/A";
                model = "N/A";
                category = "N/A";
                yom = -1; 
                odoValue = -1;
                log = "N/A";
                dailyRate = -1;
                    
                }
            
            vehicleDetails(std::string newLicense, std::string newMake, std::string newModel, std::string newCategory, int newYOM, int newODO, int newRate,
            std::string newLog)
            {
                licenseNum = newLicense;
                make = newMake;
                model = newModel;
                category = newCategory;
                yom = newYOM; 
                odoValue = newODO;
                log = newLog;
                dailyRate = newRate;
            }


        };


 enum Choice{
                PLATE, MAKE, MODEL, CATEGORY, YOM, ODO, RATE, LOG
            };

class Vehicle{
    private:
        string vin;  //unique vehicle ID distributed by manufacturer, separated to restrict modification
        vehicleDetails theDeets;
    public:
        Vehicle(string newVIN, vehicleDetails newDeets) : vin(newVIN), theDeets(newDeets){}
        Vehicle(std::string newRecord);  //for TSV file records
        Vehicle(){
            vin = "N/A";
            theDeets = vehicleDetails();
        }
        
        
        std::string outputRecord();
        string getVin();
        vehicleDetails& getDetails();
        void modifyRecord(Choice ch, string value);  //enumerator to set vehicle details
            


};

