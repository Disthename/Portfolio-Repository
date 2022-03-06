#include "battleship.h"
#include <iostream>
#include <ctime>
#include<string>
using namespace std;

//this is a friend function to Location class.
bool compare(const Location& loc1, const Location& loc2) {
    return (loc1 == loc2);

}

//overloding the << operator to teach the compiler how to print a ship info using cout
ostream& operator<<(ostream& os, const Ship& sh)
{
    string status;
    if (!sh.sunk)
        status = " up ";
    else
        status = " sunk ";
    sh.loc.print();

    os << "->" << status;
    return os;
}

//--------Location implementaton--------------------//
//rule of three
Location::Location() {
    coordinates = new int[2];
    coordinates[0] = -1;
    coordinates[1] = -1;
}
Location::Location(int x, int y) {
    coordinates = new int[2];
    coordinates[0] = x;
    coordinates[1] = y;
}
Location::Location(const Location& location) {
    coordinates = new int[2];
    coordinates[0] = location.coordinates[0];
    coordinates[1] = location.coordinates[1];
}
Location& Location::operator=(const Location& loc) {
    if (this != &loc) {
        delete[] coordinates;
        coordinates = new int[2];
        coordinates[0] = loc.coordinates[0];
        coordinates[1] = loc.coordinates[1];
    }
    return *this;
}
Location::~Location() {
    delete[] coordinates;
}
void Location::pick() {
    srand(time(nullptr) + rand() % 100);
    coordinates[0] = rand() % 5 + 1;
    coordinates[1] = rand() % 5 + 1;
}
void Location::fire() {
    do {
        cout << "Enter X coordinate (1-5): " << endl;
        cin >> coordinates[0];

        if (getCoordX() < 1 || getCoordX() > 5)
            cout << "Error: Please enter a valid coordinate (1-5): " << endl;

    } while (getCoordX() < 1 || getCoordX() > 5);
    do {
        cout << "Enter Y coordinates: " << endl;
        cin >> coordinates[1];

        if (getCoordY() < 1 || getCoordY() > 5)
            cout << "Error: Please enter a valid coordinate (1-5): " << endl;

    } while (getCoordY() < 1 || getCoordY() > 5);
}
void Location::print() const {
    cout << "[" << coordinates[0] << "," << coordinates[1] << "]";
}



//--------Ship implementaton--------------------//
Ship::Ship() {
    sunk = false;
}

bool Ship::match(const Location& locX) const {
    return compare(loc, locX);
}

void Ship::sink() {
    sunk = true;
}

void Ship::setLocation(const Location& currentLoc) {
    loc = currentLoc;
}

void Ship::printShip() const {
    cout << *this;
}

////--------Fleet implementaton ------------------//

int Fleet::check(const Location& loc) const {

    for (int i = 0; i < FLEET_SIZE; i++) {
        if (ships[i].match(loc))
            return i;
    }
    return -1;
}
void Fleet::deployFleet() {
    for (int i = 0; i < FLEET_SIZE; ++i) {
        Location loc;
        loc.pick();
        ships[i].setLocation(loc);
    }
}

bool Fleet::operational() const {
    bool flag = false;
    for (int i = 0; i < FLEET_SIZE; ++i) {
        if (!ships[i].isSunk())
            flag = true;
    }
    return flag;
}

int Fleet::isHitNSink(const Location& loc) {
    for (int i = 0; i < FLEET_SIZE; ++i) {
        if (ships[i].match(loc) && ships[i].isSunk())
            return 2;
        if (ships[i].match(loc)) {
            ships[i].sink();
            return 1;
        }

    }
    return 0;
}

void Fleet::printFleet() const {
    for (int i = 0; i < FLEET_SIZE; ++i) {
        cout << ships[i];
    }
    cout << endl;
}

Ship* Fleet::getShipList() {
    return ships;
}

int Fleet::getFleetSize() {
    return FLEET_SIZE;
}

