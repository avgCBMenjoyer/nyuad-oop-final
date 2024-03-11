#include "records.h"


using namespace std;
int counter = 0;
//NOTE: Records stored in TSV must be in the follwing format: License Plate, Customer License Number, First Name, Booking Status, Rent Date, Due Date, Return Date, Final Charge




string createDateString(tm newTm){  //create a date string
        int d = newTm.tm_mday, m = newTm.tm_mon + 1, y = newTm.tm_year + 1990;
        string hehe = to_string(d) + to_string(m) + to_string(y);
        return hehe;
    }
string Records::outputRecord(){//output string record for TSV 

    
    string returnRecord = myDetails.licensePlate + "\t" + myDetails.custDL + "\t" + myDetails.custFName + "\t" + myDetails.bookStatus + "\t" + myDetails.rentDate + "\t" + 
    myDetails.endDate + "\t" + myDetails.returnDate + "\t" + to_string(myDetails.finalCharge);

    return returnRecord;
    

}

bool Records::dateConflict(string startTm, string endTm){
    if((startTm <= myDetails.endDate) && (myDetails.rentDate <= endTm)){return true;}
    else{return false;}
}

void Records::modifyRecord(recChoice ch, string value){  //modify records using enumerator 
            switch(ch)
            {
                case PLATE:
                    myDetails.licensePlate = value;
                    break;
                case DL:
                    myDetails.custDL = value;
                    break;
                case FNAME:
                    myDetails.custFName = value;
                    break;
                case STATUS:
                    myDetails.bookStatus = value;
                    break;
                case START:
                    myDetails.rentDate = value;
                    break;
                case END:
                    myDetails.endDate = value;
                    break;
                case RETURN:
                    myDetails.returnDate = value;
                    break;
                case FINAL:
                    myDetails.finalCharge = stod(value);
                    break;
            }

        }



