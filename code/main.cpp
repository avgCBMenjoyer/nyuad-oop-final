#include "system.h"
#include <iostream>

void outMenu(){ //list of choices printed every time the user returns to the main menu
    cout << borders;
    cout << "MENU: \n";
    cout << borders;
    cout << string() + "Rent a Vehicle: \t" + "(1)\n";
    cout << string() + "Return a Vehicle: \t" + "(2)\n";
    cout << string() + "Modify Booking: \t" +"(3)\n";
    cout << string() + "View Maintenance Log: \t" + "(4)\n";
    cout << string() + "Modify Vehicle: \t" +"(5)\n";
    cout << string() + "Add New Vehicle: \t" +"(6)\n";
    cout << "Exit: \t(0)\n";
    cout << borders;
}


int main(){
    startup();
    cout << "*********************************\n";
    cout << "          ACME VEHICLES          \n";
    cout << "*********************************\n";
    outMenu();
    cout << "Enter Choice: \n";
    int myChoice;
    cin >> myChoice;
    //checks for valid choice inputs and exit choice
    while(myChoice!=0){
        switch ((myChoice))
        {
        case 1:
            makeBooking();
            break;
        case 2:
            returnVehicle();
            break;
        case 3:
            modifyBooking();
            break;
        case 4:
            showMaintenance();
            break;
        case 5:
            modifyVehicleRecords();
            break;
        case 6:
            addVehicle();
            break;
        default:
            cout << "Invalid Choice. Please try again...\n";
            cin.clear();
            break;
        }
        cin.clear();
        update();
        outMenu();
        cout << "Enter Choice: \n";
        cin >> myChoice;
    }
    
    return 0;

}