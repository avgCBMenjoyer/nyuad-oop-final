# Final Report - Rental System for Acme Vehicles Inc.

## Overview

This program demonstrates a vehicle rental management system, where the user can create and manage rental bookings for the vehicle fleet.  



## Table of Contents
1. [Introduction](#Introduction)
2. [Objective](#Objective)
3. [Requirements](#requirements)
4. [System Operations](#System)
5. [Code Structure](#code-structure)
6. [Object-Oriented Programming (OOP) Concepts](#object-oriented-programming-oop-concepts)
7. [Optimization Concepts](#optimization-concepts)
8. [Key Features](#key-features)
9. [Acknowledgements](#acknowledgements)
10. [Bibliography](#bibliography)


## Introduction
- Acme Vehicles Inc. is a vehicle rental company based in New York, Texas.
- Initially, the booking system relied on a few Excel spreadsheets where the clerks had to manually add and modify records (they had to ensure any new bookings did not clash with other previous bookings).
- After the last Federal Trade Commission lawsuit propagated by a careless intern misclicking the wrong Excel records (thanks, *Andrew*), the risks of relying on manually updated spreadsheets were highlighted extensively to senior management.
- Under a new directive from the CEO, construction of a new system incorporating data entry protections and better record storage was requested.


## Objective

- The objective of this exercise was to create a terminal-based system for the rental clerks, allowing them to efficiently manage their ever-growing vehicle
fleet and rising customer base.
- Functionality would be similar to a Global Data System (GDS) used by airlines and accomodation companies worldwide. 


## Requirements

- The system should allow for each vehicle to be booked for multiple date ranges.
- Each customer should be able to book multiple vehicles.
- The system should account for vehicles under maintenance.
- Any bookings made should be prevented from being entered if the date range conflicts with a preexisting booking.

## System

### Booking Structure
- The user inputs a date range specified by the customer to initiate the booking.
- The system returns a list of vehicles available within that date range.
- The user can present these options to the customer and select the desired vehicle.
- The user inputs the customer's drivers' licence number - if their record is already not in the system, a new one will be created.
- After the customer has returned the vehicle, the user can calculate the total charge and end the booking.

The following flowchart illustrates the program's operations for booking:

![Booking Flowchart](/images/bookingflow.drawio.svg)


The following flowchart illustrates the return operation:


![Return Flowchart](/images/returnflow.drawio.svg)

### Booking Modification

As with all service industries, the customer may change their mind and request a change in the booking dates. In order to prevent the high likelihood of creating a conflict with
another customer's booking, the system will check against the records and prevent the user from changing the record with the date range provided.

The following flowchart illustrates the program's modification operations:

![Modification Flowchart](/images/modifyflow.drawio.svg)


### Storage
- The system utilizes Tab-Separated-Variable (TSV) files, with one file for Vehicle records, one file for Customer records and one file for Booking Records. 
- The storage structure mimics the normalization forms used in relational database systems, with each record containing a key identifier from the customer and vehicle records to allow for faster referencing from each record.
- The use of TSV allows for commas to be added to the maintenance records and for the records to be accessed using any spreadsheet tool (i.e. for future auditing purposes).

The following snippet is an example list of booking records:

```js
License Plate, Customer License Number, First Name, Booking Status, Rent Date, Due Date, Return Date, Final Charge

95-6800	NZ433221	Ayumu	Archival	2024-02-11	2024-02-22	2024-02-27	3120.000000
19-1206	TN339988	Uday	Reserved	2024-02-28	2024-03-05	N/A	-1.000000
44-5544	NY224433	John	Archival	2024-05-06	2024-05-12	2024-02-27	6900.000000
95-6800	NZ433221	Ayumu	Reserved	2024-03-04	2024-03-15	N/A	-1.000000
32-3299	HK775544	Thomas	Archival	2024-02-15	2024-02-25	2024-02-27	1872.000000
32-3299	TN339988	Uday	Reserved	2024-03-05	2024-03-10	N/A	-1.000000
```

The normalization of data storage in the same manner as a database allows for one customer to have multiple vehicles reserved, and for each vehicle to be reserved for multiple dates.

[Normalization](https://www.javatpoint.com/dbms-normalization)

## Code Structure

*This system was built using the C++17 standard to ensure increased backwards compatibility.*

### Classes

The program implements three classes based on the relevant objects:
-  **Vehicle** (`vehicle.cpp`)
-  **Customer** (`customer.cpp`)
-  **Records** (`records.cpp`)

Each class contains a unique `struct` (where the attributes are stored) and an instance identifier.

>The `struct` definition is publicly accessible, however the class's unique `struct` instance is private.  
>This allows for a single getter to send all the attributes at once, reducing complexity while maintaining privatized data.

***
### Vehicle
The `Vehicle` class contains the following fields:
```c++
class Vehicle{
    private:
        string vin;  //unique vehicle ID distributed by manufacturer, separated to restrict modification
        vehicleDetails theDeets;
}
```
with the `vehicleDetails` struct containing these following attributes:

```c++
struct vehicleDetails{  
            std::string licenseNum; //License Plate number
            std::string make; //e.g. Nissan, Toyota, Norththrop Grumman
            std::string model; //e.g. Sunny, LLV, Sierra
            std::string category; //e.g Van, SUV, Sedan, APC
            std::string log;  //check for roadworthiness
            int yom; //year of manufacture
            int odoValue;  //odometer value
            int dailyRate; //Daily Rate should be whole AED
}
```  
***  
### Customer
The `Customer` class contains the following fields:

```c++
class Customer{
    private:
        inline static int counter;
        int custID;  //only used for internal counts, customers are uniquely identified by their DL number
        custDetails myDetails;
}
```
with the `custDetails` struct containing these fields:

```c++
struct custDetails{ //Customer Details should be stored in the following format: First Name, Last Name, DOB, Drivers License Number
    string fName; //First Name
    string lName; //Last Name
    string dob; //Date of Birth
    string licenseNo; //Drivers' Licence Number
}
```  
***
### Records
The `Records` class contains the following members:
```c++
class Records{
    private:
        inline static int counter;
        const int bookingID;
        recordDetails myDetails;
}
```
with the `recordDetails` struct containing these fields:
```c++
struct recordDetails{

    string licensePlate; //Vehicle Plate Num
    string custDL; //Customer's Drivers' Licence #
    string custFName; //First Name
    string bookStatus;  //"Reserved", "Archival" (mark to ignore, archival purposes only)
    string rentDate; //Start of booking
    string endDate; //End of booking
    string returnDate; //Actual return date
    double finalCharge; //Record of final charge
}
```

***

### System Interface 

In addition to the classes, a **System Interface** file (`system.cpp`) is used to conduct the booking operations and to interface with the terminal main menu. 

The `system.cpp` contains the following functions: 

```c++
//reads TSV file (template function allows for any registry of any class type to be read)
template <typename T>
void readReg(vector<T> &v, string fileName);

//writes to TSV file (template function allows for any registry of any class type to be written to)
template <typename T>
void readReg(vector<T> &v, string fileName);

inline void startup(); //updates internal vector registries from the stored data
inline void update(); //updates stored TSV files from internal vector registries

//return selection of vehicles available within specified date range
vector<Vehicle> showAvailableVehicles(string startDate, string endDate);  
void showMaintenance(); //show maintenance log
void makeBooking(); //make a new booking
Customer selectCustomer(); //check for existing customer or make new customer record
void returnVehicle(); //update records when vehicle is returned
void modifyVehicleRecords(); //update vehicle details
void modifyBooking(); //change booking dates
void addVehicle(); //add new vehicle to registry
```
***

### Ancillary Functions

To reduce the size of the `system.cpp` file, an **Ancillary Functions** file (`ancillary.cpp`) contains the supporting functions which are not accessed by `main.cpp`, along with the internal vector registries for `Vehicle`, `Customer` and `Records`.

The `ancillary.cpp` file contains the following functions and registries:

```c++
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
```



***

### Main Menu

The `main.cpp` file contains the elements of the main menu which is printed after every operation and links the user with the functions in `system.cpp`. 

![CLI Menu](/images/menucli.png)

***

### Structure Overview

The following diagram illustrates how the program files and classes are connected:

![Class Diagram](/images/structure.drawio.svg)



## Object-Oriented Programming (OOP) Concepts

The use of classes to define the relevant objects (`Vehicle`, `Customer` and `Records`) and the use of Abstract Data Types (`structs`) for managing the object details illustrate two concepts of the OOP paradigm.  
Some other examples include:

### Template Functions 

For the `system.cpp` reading and updating functions, rather than having two functions for each object, these operations were consolidated into template functions to reduce repetitive code. The following snippet highlights the `readRegistry()` function:

```c++
//reads TSV file (template function allows for any registry of any class type to be read)
template <typename T>
void readReg(vector<T> &v, string fileName){
    ifstream storedReg(fileName); //reads from TSV
    string newLine;
    while(getline(storedReg, newLine)){

        v.push_back(T(newLine)); //pushes to vector
    }
    storedReg.close();
    
}
```
These template functions take into account the different objects within the system, allowing for the program to accomodate the application of the same logic for the different class types.
***
### Data Encapsulation 

For each class, the attributes are made private and cannot be directly accessed from the instance. This ensures that the class attributes cannot be modified without calling the wrapped functions, maintaining a layer of security between an instance and the rest of the code.  
In this system, while the attributes are stored using the pubicly declared `struct`s as mentioned above, the actual data can only be accessed by passing their own stored `struct` instance through their getter. Such an example can be demonstrated by analyzing the `showMaintenanceLog()` function in `system.cpp`:

```c++
 for(auto &i: vehicleReg){
        vehicleDetails temp = i.getDetails(); //copy vehicle's details into local instance
        if(temp.log != "OK"){  //if there is a technical entry
            
            cout << i.outputRecord();
            cout << "\n" << borders;
        }
    }
```
- The function cycles through all the `Vehicle`s in the vehicle registry vector, then copies the details into its own local `vehicleDetails` instance, allowing the function to check whether any entries have been made into the maintenance log for that vehicle. 
- The objective of the `struct` declaration is to make accessing the copied data easier without relying on numerous getters, while also restricting direct modification which would be possible if all the members were publicly declared.

***
### Function Overloading (Polymorphism)

In cases where certain operations may have slight variances but require essentially the same logic to achieve their task, the option to overload the respective function to accomodate these variants may be used. The function `findConflict()` in `ancillary.cpp` is used twice: 
 - During a new booking in order to create a list of available vehicles by rejecting vehicles which have been reserved in the specified date range.
 - During modification of current booking records to ensure the new dates do not clash with any other bookings.

 The original `findConflict()` function has the following definition:

 ```c++
 //check for any conflicting records for specified vehicle within input date range
bool findConflict(string licensePlate, string startDate, string endDate);
 ```

 One issue faced with using the exact same function definition is that the original `findConflict()` definition does not take into account whether the clashing record is the record that is already stored. In order to avoid a false booking conflict, an overloaded definition was added to include the drivers' license number of the Customer who made the booking:

 ```c++
 //overloaded function to additionally check and disregard any conflicting records with the same customer (for modification)
bool findConflict(string licensePlate, string startDate, string endDate, string dlCheck);
 ```
[OOP Reference](https://www.cet.edu.in/noticefiles/285_OOPS%20lecture%20notes%20Complete.pdf)

## Optimization Concepts

As the system involves several data pipelines and the managing of numerous records, certain methods were implemented to optimize the program.

### Header Files

Due to the large number of functions and files, coupled with the codependency of each program file on each other, header files were implemented to allow for the function definitions to be directly accessed without separately declaring them. For example, the functions in `ancillary.cpp` could be accessed by `system.cpp` through `#include "ancillary.h"`. In turn, the `ancillary.h` file contains the references to the `Vehicle`, `Customer` and `Records` headers as shown below:

```c++
#include "customer.h"
#include "vehicle.h"
#include "records.h"
```
The following diagram illustrates how the access of functions traverses through the files:

![headerOverview](/images/headerflow.drawio.svg)

***
### Modular Functions

A significant part of the system involves the use of repetitive logic. Rather than replicate the logic several times within a single function, certain repetitive tasks are relegated to smaller functions (mainly the functions in `ancillary.cpp`).  
One example is the `getDate()` function, where every time a date is requested from the user, the string has to be sanitized to reduce the likelihood of the code breaking. Since date strings are frequently requested by multiple functions, the task was relegated to a singular function. An example of the repetitive logic is the string format checking logic:

```c++
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
```

If this logic were to be copied for every instance a date string is requested, the number of lines of code would practically double, in addition to greatly increasing the difficulty in debugging (i.e. if one of the copies is missing a line of code).

***
### Makefile

With the relatively large number of source files, a makefile was created for faster compilation and ensures that any required dependencies are accounted for. This has been primarily added to aid any future developers should any code require modification.

The alternative would be to list out all the `.cpp` files when compiling the executable:

```shell
g++ -o main customer.cpp main.cpp records.cpp system.cpp vehicle.cpp ancillary.cpp
```

In addition to simplifying compilation and ensuring any dependencies are included (i.e. the gnu++17 version is specified here), the makefile checks which files are dependent on another and only recompiles the neccessary files without recompiling the whole program.
***

### Exception Handling

When the user inputs a date string, if the system accepts a string which is not compatible with C++'s `ctime` library, there may be errors down the line when the booking records are managed (i.e. if Achutya enters "31-02-2024" and the system attempts to compare the current date with the converted string, it may throw an exception and subsequently crash the program.)  
To resolve this issue, the `getDate()` function in `ancillary.cpp` has a `try{} catch{}` method:

```c++
 while(!validDate){ 
        try{
            //**This code snippet is excluding the string format checker specified later**

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
 }
```
By including this `try{} catch{}` block, the system can accomodate for human error without simply crashing and forcing the user to restart the program.

***
### Enumeration

While the publicly defined `structs` for each class allow the system functions to retrieve their details in a single data container, modifying an instance's data requires the extensive use of pointers without the creation of several getter functions. To overcome this, enumerators or `enums` were defined for each class and a secondary getter function which operated using these `enums` were created. One example is the `enum`-based `modifyDetails()` function in `vehicle.h`, which makes use of the following defined `enums` for `Vehicle`:

```c++
enum Choice{
                PLATE, MAKE, MODEL, CATEGORY, YOM, ODO, RATE, LOG
            };
```
This allowed for more streamlined modification of the member variables without declaring them all as public or making several lengthier getter functions. It also plays a crucial role in the  `modifyVehicle()` function in `system.cpp`, as the desired attribute can be selected by using a simple index number inputted by the user:

```c++
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
```
In the above code snippet, the user, after being prompted by a menu to enter an index number associated with a `Vehicle` field to edit, gives an index number which is converted to its respective `enum` value and the new data is passed on to the `modifyVehicle()` function.

![Vehicle Modification Menu](/images/modifyvehicle.png)

In this example, the index '8' is associated with the 8th `enum`, namely `LOG`.

[Optimization Reference](https://github.com/x-labs-xyz/public--OOP-Spring24/tree/master)

## Key Features

### Tab-Separated Strings
All the main classes contain functions to generate tab-separated strings of their attributes, allowing for a single function call when writing to their respective TSV files. The following snippet illustrates the `outputRecord()` function for `Vehicle`:

```c++
//TSV RECORDS FOR VEHICLES MUST FOLLOW THE FORMAT: VIN, License, Make, Model, Category, YOM, ODO, Daily Rate, Maintenance Log
    std::string Vehicle::outputRecord(){  //outputs string record for TSV
        string returnRecord = ((vin)) + "\t" + theDeets.licenseNum + "\t" + theDeets.make + "\t" + theDeets.model + "\t" + theDeets.category + "\t"
         + (to_string(theDeets.yom)) + "\t" + (to_string(theDeets.odoValue)) + "\t" + (to_string(theDeets.dailyRate)) + "\t" + theDeets.log;
        return returnRecord;
    }
```

The strings produced have their attributes separated by `\t` to insert a Tab for TSV file support, in addition to reducing the complexity when printing the record to the terminal (no `setw()` required at output).

In addition, each class header contains a column label string which also contains tab separation. The following string is for the `Vehicle` class:

```c++
//Vehicle column headers for console output
#define vehicleCols std::string()  + "Plate\t" + "Make\t" + "Model\t" + "Category\t" + "YOM\t" + "Odometer(KM)\t" + "DailyRate(AED)\t" + "MaintenanceLog\n"
```

This combination allows for relatively few lines of code to display all the records (i.e. for the `showAvailableVehicles()` function when displaying available vehicles):

```c++
cout << "Index\t" << vehicleCols; //vehicle column header string
    cout  << borders; //defined border string (--------)
    for(auto &i: vehicleReg){
        vehicleDetails temp = i.getDetails();
        //check for any pending technical faults and whether there are any booking conflicts
        if(temp.log == "OK" && !findConflict(temp.licenseNum, startDate, endDate)){
            string tempStr = i.outputRecord();
            tempStr.erase(0, vinSize+1); //remove VIN from output
            cout << counter << "\t" << tempStr << "\n"; //output record to console
        }
    }
```
![Vehicle Menu](/images/vehiclemenu.png)
***
### Find Conflict

Perhaps the most important function in the entire system is the `findConflict()` function in `ancillary.cpp`. When a booking date range is given, it checks whether the given vehicle plate number has a record in the `Records` registry that overlaps with the given date range, returning `true` if a conflict is detected. 

```c++
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
```

This function makes use of the `dateConflict()` function in `records.cpp`, which returns `true` if both:
- The instance's start date is less than or equal to the given end date
- The given start date is less than or equal to the instance's start date

```c++
bool Records::dateConflict(string startTm, string endTm){
    if((startTm <= myDetails.endDate) && (myDetails.rentDate <= endTm)){return true;}
    else{return false;}
}
```
As the date strings are formatted to the ISO 8601 standard, this comparison is understood by the compiler.

#### *Example*  
>Suppose Mujtaba wishes to rent out the GMC Sierra (Plate No: 54-2099) from the 5th of June to the 10th of June. Ordinarily, if he were to put down that date, the following options would appear:

>![muji](/images/mujichoice.png)

>However, Uday has also booked the GMC Sierra, from the 8th of June to the 15th of June (record from `recordRegistry.tsv`):

>```js
>54-2099	TN339988	Uday	Reserved	2024-06-08	2024-06-15	N/A	-1.000000
>```

>This results in the GMC Sierra being flagged by `findConflict()` and therefore not presented as an option for Mujtaba:

>![nosierra](/images/mujinosierra.png)

The `findConflict()` function is similarly used when modifying booking records in order to prevent accidentally modifying a valid booking with new dates that overlap existing bookings.
***

### Maintenance Log

The `log` in `vehicleDetails` allows vehicles with technical defects to be marked by the system and not presented as an option for new bookings.

This is done by checking the `log` field - only vehicles marked `OK` are displayed (example in `showAvailableVehicles()`):

```c++
//check for any pending technical faults and whether there are any booking conflicts
        if(temp.log == "OK" && !findConflict(temp.licenseNum, startDate, endDate)){}
```

#### *Example*  
>The `vehicleRegistry.tsv` file contains the following records:

>```js
>2C8GF68464R177119	19-1206	Nissan	Sunny	Sedan	1989	125000	65	OK
>2HKRM4H57DH638807	32-3299	Chevy	Blazer	SUV	2004	225432	120	OK
>1J4GW58S61C648209	95-6800	Toyota	Sequoia	SUV	2012	46000	150	OK
>1GCDM19Z2RB132255	44-5544	Toyota	HiAce	Van	2009	214000	100	Alternator Belt Loose
>1FTFW1ETXBFC04760	54-2099	GMC	Sierra	SUV	2001	102420	120	OK
>```
>The Toyota HiAce Van (44-5544) has a loose alternator belt, therefore it does not appear in the list of Vehicles available when booking:

>![availableNoDefect](/images/mujichoice.png)

>Additionally, it is flagged by the `showMaintenanceLog()` and appears when the maintenance log is selected by the user:

>![Tech Log](/images/techlog.png)

***
### Fare Calculation

The `calculateFare()` function in `ancillary.cpp` takes two date strings, splits them into their relevant sections (day, month, year) and uses `mktime()` to convert the dates into seconds before calculating the difference, converting into number of days and then multiplying the result by the daily fare assigned to each vehicle in `vehicleDetails`.

```c++
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
```
*As the system was built to the C++17 standard, the `std::chrono` functions specific to dates in C++20 were not implemented*

***
### Current Date

The `todayDate()` function in `ancillary.cpp` receives the local machine time and parses it into a ISO 8601 string:
```c++
string todayDate(){  //returns a string of today's date based on system clock
    tm currentDate;
    time_t curr = time(0); //current time
    currentDate = *localtime(&curr); //gets timezone-based time
    stringstream ss;
    ss << put_time(&currentDate, "%F"); //converts to YYYY-MM-DD
    string theDate = ss.str();
    return theDate;
}
```
***
### Attribute Modifier List

Taking advantage of the tab-spaced headers, for the `modifyVehicle()` function the attributes were listed by splitting the `vehicleCols` defined string and listing them one-by-one with an associated index number:

```c++
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
```

This combined with the `enum` declared for the `Vehicle` class (the index inputted can directly be converted to the respective `enum`) allows the specific vehicle attributes to be displayed and modified without several lines of code for a dedicated menu:

![Vehicle Modification Menu](/images/modifyvehicle.png)

### Adding New Vehicle

Using similar methods to the `modifyVehicle()` function, the `addVehicle()` function in `system.cpp` prompts the user for all the vehicle attributes and pushes a newly created `Vehicle` instance into the internal vehicle registry.

```c++
stringstream ss(vehicleCols); //outputs list of attributes by splitting vehicle column header declared in "vehicle.h"
    while(getline(ss, item, '\t')){
        cout << "Enter " << item << ": \n";
        cin >> temp;
        newVehicle += ("\t" + temp);
    }
```
## Acknowledgements

I would like to acknowledge my peers Andrew Surendran, Uday Menon, Mujtaba Kamran and Asini Subanya for providing the emotional support necessary to construct this relatively complicated program.

## Bibliography

- https://cplusplus.com/reference/ctime/strftime/
- https://stackoverflow.com/questions/997946/how-to-get-current-time-and-date-in-c
- https://stackoverflow.com/questions/4324763/can-we-have-functions-inside-functions-in-c
- https://stackoverflow.com/questions/17319579/how-to-compare-2-dates-in-c
- https://stackoverflow.com/questions/14218894/number-of-days-between-two-dates-c
- https://www.geeksforgeeks.org/finding-number-of-days-between-two-dates-using-stringstream/
- https://stackoverflow.com/questions/3019153/how-do-i-use-an-enum-value-in-a-switch-statement-in-c
- https://www.altexsoft.com/blog/car-rental-reservation-system/
- https://github.com/x-labs-xyz/public--OOP-Spring24/tree/master
- https://www.cet.edu.in/noticefiles/285_OOPS%20lecture%20notes%20Complete.pdf
- https://en.cppreference.com/w/cpp/language/enum
- https://en.cppreference.com/w/cpp/language/inline
- https://www.javatpoint.com/dbms-normalization

*ChatGPT was NOT consulted during the entirety of this project*
