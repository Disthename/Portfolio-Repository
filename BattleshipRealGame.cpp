// BattleshipRealGame.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "battleship.h"
using namespace std;

bool uniqueLocations(Fleet myFleet) {
    //Pre-conditionds: myFleet object must have a valid array of ship data type.
    Ship* fleetInfoList = myFleet.getShipList();
    for (int i = 0; i < myFleet.getFleetSize(); ++i)
        for (int j = i + 1; j < myFleet.getFleetSize(); ++j) {
            if (fleetInfoList[i].getLocation() == fleetInfoList[j].getLocation())
            {
                return false;
            }

        }
    return true;
}

int main()
{
    Fleet myFleet;
    myFleet.deployFleet();
    while (!uniqueLocations(myFleet)) {
        myFleet.deployFleet();
    }
    
    cout << "Should ship positions be printed? (y/n): " << endl;
    char choicePlayer;
    cin >> choicePlayer;
    if (choicePlayer == 'y' || choicePlayer == 'Y')
        myFleet.printFleet();
    while (myFleet.operational() == true) {
        Location myLocation;
        myLocation.fire();
        int num = myFleet.isHitNSink(myLocation);
        if (num == 0) {
            cout << "Miss! " << endl;
        }

        else if (num == 1) {
            cout << "Hit! " << endl;

        }

        else
            cout << "Ship in this location has already been sunk. " << endl;
    }
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
