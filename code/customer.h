#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>


//Customer column headers for console output
#define custCols std::string() + "First Name\t" + "Last Name\t" + "DOB\t" + "Drivers License Number\n"

using namespace std;

struct custDetails{ //Customer Details should be stored in the following format: First Name, Last Name, DOB, Drivers License Number
    string fName; //First Name
    string lName; //Last Name
    string dob; //Date of Birth
    string licenseNo; //Drivers' Licence Number

    custDetails(string newFirst, string newLast, string newDOB, string newLicense){
        fName = newFirst;
        lName = newLast;
        dob = newDOB;
        licenseNo = newLicense;
    }
    custDetails(){
        fName = "N/A";
        dob = -1;
        licenseNo = "N/A";
    }

};

class Customer{
    private:
        inline static int counter;
        int custID;  //only used for internal counts, customers are uniquely identified by their DL number
        custDetails myDetails;
    public:
        
        Customer():custID(++counter), myDetails(custDetails()){}
        Customer(custDetails newDetails) : custID(++counter), myDetails(newDetails){}
        Customer(string newRecord){  //for adding records from TSV
            std::istringstream uwu(newRecord);
            vector<string> theItems;
            string token;
            while(getline(uwu, token, '\t')){
                    theItems.push_back(token);
            }
            myDetails = (custDetails(theItems[0], theItems[1], theItems[2], theItems[3]));
        }  
        void printDeets();
        string outputRecord();
        custDetails& getDetails(){
            return myDetails;
        } 
        int returnID(){
            return custID;
        }  
        
        
        

};
