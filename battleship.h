// class definitions for battleship assignment

#ifndef BATTLESHIP_H_
#define BATTLESHIP_H_

#include <cstdlib>
#include <ctime>
#include <string>
#include <iostream>
using namespace std;
// coordinates (location) of the ship and shots
class Location {
public:
    Location(); // void constructor, dynamically allocates the coordinates array and assignes -1 to both of the coordinates
    Location(int, int);// overloaded constructor to be used to create a location object with given coordinates.
    void pick(); // picks a random location. Means assignes 0 to 4 to the elements of the coordinates array
    void fire(); // asks the user to input coordinates of the next shot
    void print() const; // prints location in format "a1"
    int getCoordX() { return coordinates[0]; }
    int getCoordY() { return coordinates[1]; }
    // predicate returns true if the two locations match
    friend bool compare(const Location&, const Location&);
    //rule of three
    Location(const Location&); // copycostructor
    Location& operator=(const Location&); //assignment overloading
    ~Location();//destructor

    //overloading ==
    bool operator==(Location rhs) const {
        return (coordinates[0] == rhs.coordinates[0] && coordinates[1] == rhs.coordinates[1]);
    }

private:
    static const int FIELD_SIZE = 5; // the field (ocean) is fieldSize X fieldSize
    int* coordinates;
};

// contains ship's coordinates (location) and whether is was sunk
class Ship {
public:
    Ship(); // void constructor, sets sunk=false
    bool match(const Location&) const; // returns true if this location matches
                                      // the ship's location
    bool isSunk() const { return sunk; } // checks to see if the ship is sunk
    void sink();       // sets "sunk" member variable of the ship to true
    void setLocation(const Location&); // deploys the ship at the specified location
    void printShip() const; // prints location and status of the ship
    Location getLocation() { return loc; }
    /*   ostream& operator<<( ostream& os)
       {
           string status;
           if (!sunk)
               status = " up ";
           else
               status = " sunk ";
           loc.print();

           os << "->" << status;
           return os;
       }*/

    friend  ostream& operator<<(ostream& os, const Ship& sh);

private:
    Location loc;
    bool sunk;
};

// contains the fleet of the deployed ships
class Fleet {
public:
    void deployFleet(); // deploys the ships in random locations
                            // of the ocean
    bool operational() const; // predicate returns true if at least
                              // one ship in the fleet is not sunk
    int isHitNSink(const Location&); // returns 1 if there was a deployed
                          // ship at this location (hit) and sinks it
                          // and returns 0  if a miss(not occupied by any ship)
    //returns 2 if it is occupied by it is already sunk!
    void printFleet() const; // prints out locations of ships in fleet
    Ship* getShipList();
    int getFleetSize();
private:
    static const int FLEET_SIZE = 5; // number of battleships
    int check(const Location&) const;  // returns index of the ship 
                                  // that matches location
                                  // -1 if none match
    Ship ships[FLEET_SIZE];        // battleships of the fleet
};

#endif /* BATTLESHIP_H_ */#pragma once
