
#include "customer.h"
using namespace std;



//Customer Details should be stored in the following format: First Name, Last Name, DOB, Drivers License Number


string Customer::outputRecord(){
    string returnRecord = myDetails.fName + "\t" + myDetails.lName + "\t" + myDetails.dob + "\t" + myDetails.licenseNo;
    return returnRecord;
}

