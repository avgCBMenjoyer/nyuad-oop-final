#include "ancillary.h"




using namespace std;

string getDate(){  //enter a valid date that can be represented in UTC
    tm myDate;
    string myInput, myStr, failed="Bad Date.";
    cout << "Enter Date (YYYY-MM-DD): " << endl;
    cin >> myInput;
    bool validDate = false;
    while(!validDate){ 
        try{
            stringstream ss(myInput);
            int digitCount=0, dashCount=0;
            //check string format 
            for(auto c : myInput){
                if(isdigit(c)){
                    digitCount++;
                }
                else if(c == '-'){
                    dashCount++;
                }
                else{
                    throw(failed);
                }
            }
            if(digitCount != 8 || dashCount != 2){  //2024-01-01 (8 numbers, 2 dashes)
                throw(failed);
            }

            //check against mktime to ensure it can be converted
            ss >> get_time(&myDate, "%Y-%m-%d");
            time_t time = mktime(&myDate);
            validDate = true;
        }
        catch(exception e){
            cout << "Invalid Date.\n\n";
            cout << "Enter Date (YYYY-MM-DD): " << endl;
            cin >> myInput;
            bool validDate = false;

        }
        catch(string failed){
            cout << "Date Format Incorrect.\n";
            cout << "Press Enter to continue...\n";
            cin.ignore();
            cout << "Enter Date (YYYY-MM-DD): " << endl;
            cin >> myInput;
            bool validDate = false;

        }
    }
    cout << "\n";
    return myInput;
}




//Retrieve vehicle reccord based on plate number
Vehicle& searchVehicle(string plateNo){
    Vehicle out;
    for(auto &i : vehicleReg){
        vehicleDetails temp = i.getDetails();
        if(temp.licenseNum == plateNo){
            cout << "Vehicle with Plate Num: " + plateNo + " found.\n";
            return i;
        }

    }
    exit(-1);
    
}


bool findConflict(string licensePlate, string startDate, string endDate){  //used to find whether vehicle is reserved within date range
    bool conflict = false;
    for(auto &i : recordList){ //cycles through the internal Records vector registry
        recordDetails temp = i.getDeets();
        //if the "i"th record has the same plate number and is not an archival record
        if(temp.licensePlate == licensePlate && temp.bookStatus != "Archival"){
            if(i.dateConflict(startDate, endDate)){
                conflict = true;
            }
        }
    }
    return conflict;
}
//OVERLOADED FUNCTION
bool findConflict(string licensePlate, string startDate, string endDate, string dlCheck){  //used to find whether vehicle is reserved within date range and is not the same DL
    bool conflict = false;
    for(auto &i : recordList){
        recordDetails temp = i.getDeets();
        if(temp.licensePlate == licensePlate && temp.bookStatus != "Archival" && temp.custDL != dlCheck){
            if(i.dateConflict(startDate, endDate)){
                conflict = true;
            }
            
        }
    }
    return conflict;
}

double calculateFare(int vehicleFare, string startDate, string endDate){
    //using not-so-portable methods as  C++ melts down trying to use multiple stringstreams
    stringstream ss(startDate + "-" + endDate);
    int year, month, day;
    char hyphen;
    //// Parse the first date into seconds
    ss >> year >> hyphen >> month >> hyphen >> day;
    struct tm starttm = { 0, 0, 0, day,
                        month - 1, year - 1900 };
    time_t start = mktime(&starttm);
 
    // Parse the second date into seconds
    ss >> hyphen >> year >> hyphen
        >> month >> hyphen >> day;
    struct tm endtm = { 0, 0, 0, day,
                        month - 1, year - 1900 };
    time_t end = mktime(&endtm);
 
    // Find out the difference and divide it
    // by 86400 to get the number of days
    double difference = abs(end - start) / 86400;
    cout << "Number of Days: " << difference  << endl;
    return difference * vehicleFare;
}

string todayDate(){  //returns a string of today's date based on system clock
    tm currentDate;
    time_t curr = time(0); //current time
    currentDate = *localtime(&curr); //gets timezone-based time
    stringstream ss;
    ss << put_time(&currentDate, "%F"); //converts to YYYY-MM-DD
    string theDate = ss.str();
    return theDate;
}



