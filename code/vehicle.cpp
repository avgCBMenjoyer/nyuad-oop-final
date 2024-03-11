#include <iostream>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include "vehicle.h"

//TSV RECORDS FOR VEHICLES MUST FOLLOW THE FORMAT: VIN, License, Make, Model, Category, YOM, ODO, Daily Rate, Maintenance Log
using namespace std;

    Vehicle::Vehicle(string newRecord){ //converts TSV string into Vehicle instance
        std::istringstream uwu(newRecord);
        vector<string> theItems;
        string token;
        while(getline(uwu, token, '\t')){
            theItems.push_back(token);
        }
        vin = theItems[0];
        theDeets = vehicleDetails(theItems[1], theItems[2], theItems[3], theItems[4], stoi(theItems[5]), stoi(theItems[6]), stoi(theItems[7]), theItems[8]);


    }
    //TSV RECORDS FOR VEHICLES MUST FOLLOW THE FORMAT: VIN, License, Make, Model, Category, YOM, ODO, Daily Rate, Maintenance Log
    std::string Vehicle::outputRecord(){  //outputs string record for TSV
        string returnRecord = ((vin)) + "\t" + theDeets.licenseNum + "\t" + theDeets.make + "\t" + theDeets.model + "\t" + theDeets.category + "\t"
         + (to_string(theDeets.yom)) + "\t" + (to_string(theDeets.odoValue)) + "\t" + (to_string(theDeets.dailyRate)) + "\t" + theDeets.log;
        return returnRecord;
    }
    vehicleDetails& Vehicle::getDetails(){
        return theDeets;
    }
    string Vehicle::getVin(){
        return vin;
    }

    void Vehicle::modifyRecord(Choice ch, string value){  //enumerator to set vehicle details
            switch(ch)
            {
                case PLATE:
                    theDeets.licenseNum = value;
                    break;
                case MAKE:
                    theDeets.make = value;
                    break;
                case MODEL:
                    theDeets.model = value;
                    break;
                case CATEGORY:
                    theDeets.category = value;
                    break;
                case LOG:
                    theDeets.log = value;
                    break;
                case YOM:
                    theDeets.yom = stoi(value);
                    break;
                case ODO:
                    theDeets.odoValue = stoi(value);
                    break;
                case RATE:
                    theDeets.dailyRate = stoi(value);
            }
        }

   

    

        



        




