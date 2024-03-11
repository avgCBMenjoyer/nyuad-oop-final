#pragma once
#include "ancillary.h"



//reads TSV file (template function allows for any registry of any class type to be read)
template <typename T>
void readReg(vector<T> &v, string fileName){
    try{
    ifstream storedReg(fileName); //reads from TSV
    if(storedReg.fail()){
        throw(exception());
    }
    string newLine;
    while(getline(storedReg, newLine)){

        v.push_back(T(newLine)); //pushes to vector
    }
    storedReg.close();
    }
    catch(exception e){
        cout << fileName << " not found. Please contact your administrator.\n";
        exit(-1);
    }
    
}

//reads TSV file (template function allows for any registry of any class type to be written to)
template <typename T>
void updateReg(vector<T> &v, string fileName){
    try{
    ofstream storedReg(fileName);
    if(storedReg.fail()){
        throw(exception());
    }
    for(auto &i : v){
        storedReg << ((i.outputRecord())) << "\n";
    }
    storedReg.close();
    }
    catch(exception e){
        cout << fileName << " not found. Please contact your administrator.\n";
        exit(-1);
    }
}

inline void startup(){ //updates internal vector registries from the stored data
    readReg(vehicleReg, "vehicleRegistry.tsv");
    readReg(custReg, "customerRegistry.tsv");
    readReg(recordList, "recordRegistry.tsv");
}

inline void update(){ //updates stored TSV files from internal vector registries
    updateReg(vehicleReg, "vehicleRegistry.tsv");
    updateReg(custReg, "customerRegistry.tsv");
    updateReg(recordList, "recordRegistry.tsv");
}

//return selection of vehicles available within specified date range
vector<Vehicle> showAvailableVehicles(string startDate, string endDate);  
void showMaintenance(); //show maintenance log
void makeBooking(); //make a new booking
Customer selectCustomer(); //check for existing customer or make new customer record
void returnVehicle(); //update records when vehicle is returned
void modifyVehicleRecords(); //update vehicle details
void modifyBooking(); //change booking dates
void addVehicle(); //add new vehicle to registry








