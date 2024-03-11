#include "system.h"

#include <stdlib.h>
//Function to add a new vehicle to fleet
void addVehicle(){
    cout << borders;
    string newVehicle="", temp, item;
    cout << "Enter VIN: \n";
    cin >> temp;
    newVehicle += temp;
    stringstream ss(vehicleCols); //outputs list of attributes by splitting vehicle column header declared in "vehicle.h"
    while(getline(ss, item, '\t')){
        cout << "Enter " << item << ": \n";
        cin >> temp;
        newVehicle += ("\t" + temp);
    }
    cout << "\n" << borders;
    vehicleReg.push_back(Vehicle(newVehicle));
    
}

vector<Vehicle> showAvailableVehicles(string startDate, string endDate){  //Output list of available vehicles based on date range specified
    cout << "\nVEHICLES AVAILABLE: \n" << borders;
    vector<Vehicle> available;
    int counter = 1;
    cout << "Index\t" << vehicleCols; //vehicle column header string
    cout  << borders; //defined border string (--------)
    for(auto &i: vehicleReg){
        vehicleDetails temp = i.getDetails();
        //check for any pending technical faults and whether there are any booking conflicts
        if(temp.log == "OK" && !findConflict(temp.licenseNum, startDate, endDate)){
            string tempStr = i.outputRecord();
            tempStr.erase(0, vinSize+1); //remove VIN from output
            cout << counter << "\t" << tempStr << "\n"; //output record to console
            available.push_back(i);
            counter++;
            cout << "\n" << borders;
        }
        

    }
    return available;
    
}

void showMaintenance(){  //Output maintenance log
    cout << "\nMAINTENANCE LOG\n\n" << borders;
    cout << "VIN\t" << vehicleCols;
    cout << "\n" << borders;
    for(auto &i: vehicleReg){
        vehicleDetails temp = i.getDetails(); //copy vehicle's details to local instance
        
        if(temp.log != "OK"){  //if there is a technical entry
            
            cout << i.outputRecord();
            cout << "\n" << borders;
        }
        

    }
    cout << "**NOTE: To remove vehicle from maintenance log, change the log entry to 'OK'**\n";
    cout << "\n\n";
}




Customer selectCustomer(){  //Searches Customer records based on DL number and adds new record if requested (customers cannot be added without booking)
    Customer payload;
    string inputNum;
    cout << "Enter Drivers Licence Number: \n";
    cin >> inputNum;
    bool found=false;
    while(!found){
        for(auto &i:custReg){
            custDetails temp = i.getDetails();
            if(temp.licenseNo == inputNum){
                payload = i;
                found = true;
            }
        }
        if(!found){ //If the drivers licence does not match any existing customers
            string choice;
            cout << "Record Not Found. Would you like to add a new customer? (Y/N): \n";
            cin >> choice;
            if(choice == "Y" || choice == "y"){
                string fName, lName, dob, newRecord;
                cout << "Enter First Name: \n";
                cin >> fName;
                cout << "Enter Last Name: \n";
                cin >> lName;
                cout << "Enter Date of Birth (YYYY-MM-DD): \n";
                cin >> dob;
                newRecord = fName + "\t" + lName + "\t" + dob + "\t" + inputNum;
                payload = Customer(newRecord);
                custReg.push_back(payload);
                
                found = true;
            }
            else{
                cout << "Enter Drivers Licence Number: \n";
                cin >> inputNum;
                found = false;
            }

        }

    }
    return payload;
}

void makeBooking(){  //Find vehicle based off license plate and time duration
    vector<Vehicle> rangeMatch;
    cout << "Booking Date\n";
    string startDate = getDate();
    cout << "End Date\n";
    string endDate = getDate();
    rangeMatch = showAvailableVehicles(startDate, endDate); //returns list of vehicles which are not reserved
    if(rangeMatch.size()==0){
        cout << "No Vehicles Available for the selected date.\n";
        return;
    }
    cout << "Enter Index Number (Enter 0 to exit): " << endl;
    int index;
    cin >> index;
    if(index == 0){  //exit
        return;
    }
    Vehicle payload;
    bool found=false;
    //ensure correct index
    while(!found){
        if(index > 0 && index <= rangeMatch.size()){
            payload = rangeMatch[index-1];
            found = true;
        }
        else{
            cout << "Invalid index number.\n";
            cout << "Enter Index Number: " << endl;
            cin >> index;
            found = false;

        }
    }
    Customer myCust;
    string status;
    myCust = selectCustomer();
    status = "Reserved";

    //create new record and add to record registry
    //NOTE: Records stored in TSV must be in the follwing format: License Plate, Customer License Number, First Name, Booking Status, Rent Date, Return Date, Final Charge 
    vehicleDetails myChoice = payload.getDetails();
    custDetails myDetails = myCust.getDetails();
    string newBooking = (myChoice.licenseNum) + "\t" + (myDetails.licenseNo) + "\t" + (myDetails.fName) + "\t" + status + "\t" + startDate + "\t" + endDate + "\t" + "N/A" + "\t" + "-1";
    recordList.push_back(Records(newBooking));
    updateReg<Records>(recordList, "recordRegistry.tsv");
    cout << "\n";
    cout << borders;
    cout << "Booking ID (Keep for future reference): " << recordList.back().getID() << endl;
    cout << borders;

    
}


//Updates record and outputs cost
void returnVehicle(){
    int bookingRef;
    cout << "\nEnter Booking Reference: \n";
    cin >> bookingRef;
    bool found = false;
    
    while(!found){
        if(bookingRef >= 1 && bookingRef <= recordList.size()){ //if valid booking reference
                Records* temp = &recordList[bookingRef-1]; //obtains record reference to modify (not just read from)
                recordDetails* returnDeets = &((*temp).getDeets()); 
                if(returnDeets->bookStatus != "Archival"){

                    if(returnDeets->rentDate > todayDate()){  //to prevent charges if the rent date is after today's date
                        cout << "NOTE: The start date for this booking is after today. \n";
                        cout << "Please delete the booking. \n";
                        return;
                    }

                    //obtain daily rate
                    temp->modifyRecord(RETURN, todayDate());
                    cout << "Customer: " << returnDeets->custFName << endl;
                    temp->modifyRecord(STATUS, "Archival");

                    //retrieve vehicle daily rate from vehicle records
                    Vehicle* returnCar = &(searchVehicle(returnDeets->licensePlate)); //obtains vehicle reference to modify (not just read from)
                    vehicleDetails* tempDeets = &((*returnCar).getDetails());

                    //calculate fare
                    double myFare = calculateFare(tempDeets->dailyRate, returnDeets->rentDate, returnDeets->returnDate);
                    cout << "End Date: " << returnDeets->endDate << "\n";
                    if(returnDeets->returnDate > returnDeets->endDate){
                        myFare *= 1.3;  //30% surcharge if returned late
                        cout << "VEHICLE RETURNED LATE: SURCHARGE OF 30 PERCENT APPLIED.\n";
                    }
                    cout << "\n";
                    cout << borders;
                    cout << "FINAL CHARGE (AED): " << myFare << "\n";
                    cout << borders;
                    returnDeets->finalCharge = myFare;
                    
                    //Update mileage
                    int newOdo;
                    cout << "Enter Odometer Value: \n";
                    cin >> newOdo;
                    while(tempDeets->odoValue > newOdo){ //the input mileage should be greater than current value
                        cout << "The value inputted is less than the recorded mileage of " << tempDeets->odoValue << " km. Please try again.\n";
                        cin >> newOdo;
                    }
                    tempDeets->odoValue = newOdo;

                    //vehicle can be marked for maintenance
                    cout << "Would you like to report any technical issues? (Y/N): \n";
                    string userChoice;
                    cin >> userChoice;
                    if(userChoice == "Y" || userChoice == "y"){
                        string newLog;
                        cout << "Enter a description of the issues: \n";
                        cin.ignore(1);
                        getline(cin, newLog);
                        tempDeets->log = newLog;
                    }
                    else{
                        tempDeets->log = "OK";
                    }
                    cout << "Vehicle has been returned successfully. \n";
                    updateReg(vehicleReg, "vehicleRegistry.tsv");
                    updateReg(recordList, "recordRegistry.tsv");
                    found = true;
                }
                else{
                    cout << "Archived Booking Reference. Please try again. \n";
                    cout << "\nEnter Booking Reference: \n";
                    cin >> bookingRef;
                    found = false;
                }
        }
        else{
            cout << "Booking not Found. Please try again. \n";
                cout << "\nEnter Booking Reference: \n";
                cin >> bookingRef;
                found = false;
        }   
    }
    

}


void modifyVehicleRecords(){  //change vehicle records (except VIN)
    int counter = 1, index = 0, fieldIndex=0;
    cout << borders;
    cout << "Index\tVIN\t" + vehicleCols;
    cout << borders;
    for(auto &i : vehicleReg){
        cout << counter << "\t" << i.outputRecord() << "\n";
        
        cout << "\n";
        counter++;
    }
    cout << "\nSelect Index: \n";
    cin >> index;
    
    if(index <= 0 || index > vehicleReg.size()){
        cout << "Vehicle not found. \n";
        return;
    }
    //Outputs vehicle properties and an index to choose
    cout << "\nChoose which field to modify: \n";
    cout << borders;
    string tmpy;
    counter = 1;
    stringstream ss(vehicleCols); //converts vehicle details header string into stream
    while(getline(ss, tmpy, '\t')){ //splits by tab
        cout << counter << "\t" << tmpy << "\n";
        counter++;
    }
    cin.ignore(1);
    cout << "\nSelect Index: \n";
    cin >> fieldIndex;
    bool found = false;
    while(!found){
        if(fieldIndex <= 0 || fieldIndex >= counter){
            cout << "Field not found. Please try again. \n";
            cout << "\nSelect Index: \n";
            cin >> fieldIndex;
        }
        else{ //Gets enum based on field index and calls modifier function
            cin.ignore(1);
            Choice check = (Choice)(fieldIndex-1); //converts inputted index to enum value
            cout << "Enter new Value: \n";
            string newVal;
            getline(cin, newVal);
            if(check==PLATE){
                //Modify records with the current plate number to ensure they match
                string oldPlate = vehicleReg[index-1].getDetails().licenseNum;
                for(auto &j : recordList){
                    if(j.getDeets().licensePlate == oldPlate){
                        j.modifyRecord(LPLATE, newVal);
                    }
                }
            }
            vehicleReg[index-1].modifyRecord(check, newVal);
            found = true;
            
        }
    }

    


}

void modifyBooking(){
    //output all records
    cout << "\n" << borders;
    cout << "BOOKING RECORDS: \n";
    cout << borders;
    cout << "Index\t" + recordCols;
    cout << borders;
    int counter=1, index=0, choice=0;
    for(auto &i : recordList){
        cout << counter << "\t" << i.outputRecord() << endl;
        counter++;
    }

    //select record to modify
    cout << "\nSelect Index (Press 0 to exit): \n";
    cin >> index;
    
    if(index <= 0 || index > recordList.size()){ //incorrect index
        cout << "Record not found. \n";
        return;
    }
    else if(recordList[index-1].getDeets().bookStatus == "Archival"){  //record is preserved
        cout << "Modifying archival records is not allowed. \n";
        return;
    }
    cout << borders;
    cin.ignore(1);

    //Modify dates or 'delete' booking
    cout << "OPTIONS: \n";
    cout << borders;
    cout << "1. Modify Date\n";
    cout << "2. Delete Booking (record preserved)\n";
    cout << borders;
    cout << "Enter Choice: \n";
    cin >> choice;
    while(choice != 1 && choice != 2){
        cout << "Incorrect Choice. Please try again. \n";
        cout << "Enter Choice: \n";
        cin >> choice;
    }
    string newStart, newEnd;
    switch(choice){
        case 1:
            //get new dates
            cout << "New Start Date: \n";
            newStart = getDate();
            cout << "New End Date: \n";
            newEnd = getDate();

            //check for potential conflicts with other records that have the same vehicle
            while(findConflict(recordList[index-1].getDeets().licensePlate, newStart, newEnd, recordList[index-1].getDeets().custDL)){
                cout << "BOOKING CONFLICT.\n";
                cout << "Please try again: \n\n";
                cout << "New Start Date: \n";
                newStart = getDate();
                cout << "New End Date: \n";
                newEnd = getDate();
            }
            //modify using enumerators defined in record.h
            recordList[index-1].modifyRecord(START, newStart);
            recordList[index-1].modifyRecord(END, newEnd);
            break;
        case 2:
            recordList[index-1].modifyRecord(STATUS, "Archival");
            break;

    }
    cout << "Record Modified Successfully.\n\n";



}






