#pragma once
#include <iostream>
#include <sstream>
#include <fstream>
#include <ctime>
#include <string>
#include "vehicle.h"
#include "customer.h"
#include <chrono>
#include <iomanip>
#include <vector>

//Record Column header for console output
#define recordCols std::string()  + "Plate\t" + "DL Number\t" + "Name\t" + "Status\t" + "Rent Date\t" + "End Date\t" + "Return Date\t" + "Final Charge (AED)\n"

using namespace std;

//NOTE: Records stored in TSV must be in the follwing format: License Plate, Customer License Number, First Name, Booking Status, Rent Date, End Date, Return Date, Final Charge 
struct recordDetails{

    string licensePlate; //Vehicle Plate Num
    string custDL; //Customer's Drivers' Licence #
    string custFName; //First Name
    string bookStatus;  //"Available", "Reserved", "Overdue", "Archival" (mark to ignore, archival purposes only)
    string rentDate; //Start of booking
    string endDate; //End of booking
    string returnDate; //Actual return date
    double finalCharge; //Record of final charge

    recordDetails(){
        licensePlate = "N/A";
        custDL = "N/A";
        custFName = "N/A";
        bookStatus = "N/A";
        finalCharge = -1;

    }

    recordDetails(string license, string custNum, string newName, string bookEntry, string rentEntry, string endEntry, string returnEntry, double newCharge){
        licensePlate = license;
        custDL = custNum;
        custFName = newName;
        bookStatus = bookEntry;
        rentDate = rentEntry;
        endDate = endEntry;
        returnDate = returnEntry;
        finalCharge = newCharge;
    }
    
    

};

enum recChoice{
    LPLATE, DL, FNAME, STATUS, START, END, RETURN, FINAL
};

class Records{
    private:
        inline static int counter;
        const int bookingID;
        recordDetails myDetails;
    public:
        Records():bookingID(counter++){
            
        }
        Records(recordDetails newDeets):bookingID(++counter),myDetails(newDeets){}
        
        Records(string newRecord) : bookingID(++counter){  //for TSV Records
            std::istringstream uwu(newRecord);
            vector<string> theItems;
            string token;
            while(getline(uwu, token, '\t')){
                theItems.push_back(token);
            }
            

            myDetails = recordDetails(theItems[0], theItems[1], theItems[2], theItems[3], theItems[4], theItems[5], theItems[6], stod(theItems[7]));

        }
        void printRecord();
        string outputRecord();
        int getID(){
            return bookingID;
        }
        recordDetails& getDeets(){
            return myDetails;
        }
        
        bool dateConflict(string startTm, string endTm); //check for date conflict with record's start and end dates
        void modifyRecord(recChoice ch, string value);  //enum modifier
};      
