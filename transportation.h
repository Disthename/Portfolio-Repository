#pragma once
#ifndef Transportation_H
#define Transportation_H
#include <string>
#include <iostream>
#include <fstream>
#include "NodeList.h"
using namespace std;
class Vehicle {
public:
	Vehicle();
	Vehicle(int, string, string, int, bool, int, NodeList<string>);
	void reserve()  { available = 0; }
	void unReserve() { available = 1; }
	bool status() const { return available; }
	int getID() const { return vehicleID; }
	string getmake() const { return make; }
	string getmodel() const { return model; }
	int getSeats()const { return seats; }
	int getextarFeatures()const { return extarFeatures; }
	NodeList <string> getFeaturesList() const {
		return otherFeaturesList;
	}
	~Vehicle(); // destructor
	Vehicle(const Vehicle& );   //copy Constructor

	Vehicle operator=(const Vehicle& rhs);  // assignment overlaoding

	// operators overloading 
	bool operator>(const Vehicle&);
	bool operator<(const Vehicle&);
	bool operator==(const Vehicle&);
	bool operator<=(const Vehicle&);
	bool operator>=(const Vehicle&);
	bool operator!=(const Vehicle&);

	friend ostream & operator << (ostream &out, const Vehicle &);
	friend ofstream & operator << (ofstream &out, const Vehicle &);
	
private:
	int vehicleID; //an integer number automatically assigned when object is created.
	string make;
	string model;
	int seats; // number of seats
	bool available; //a Boolean variable that is initially has true indicating that the vehicle is available.
	int extarFeatures; // number of the extra features in the vehicle
	NodeList<string> otherFeaturesList;
	/*Project 6: Refactoring is needed here. You need to replace the dynamic array with a list object. 
	you must update all the affected methods in the entire project*/

};  // end of class Vehicle

//========================================================================
class Inventory {
public:
	Inventory(); 

	Inventory(int );
	//rule of three
	Inventory(const Inventory&);  //C.C
	Inventory operator=(const Inventory&); // =.O.L
	~Inventory(); 

	bool push_Back(Vehicle  );// push_back
	void sortList();
	//typename NodeList<Vehicle>::Iterator beginIter() { return v_List.begin(); }
	void printList(NodeList<Vehicle>::Iterator);
	NodeList<Vehicle>& getList() { return v_List; };
	void printResevedList(NodeList<Vehicle>::Iterator);
	NodeList<Vehicle>& getResList() { return v_List; };
	void printAvailableList(NodeList<Vehicle>::Iterator);
	NodeList<Vehicle>& getAvaList() { return v_List; };
	bool found(NodeList<Vehicle>::Iterator, int, int&);
	NodeList<Vehicle>& getFound() { return v_List; };
	bool reserveVehicle(int );
	bool returnVehicle(int );
	int checkID(int );
	int size() { return registredVehiclesNo; }  // inline implementation
	bool saveToFile(string fileName);
	//ofstream & operator<<(ofstream &outfile)
	//	// overleaoding the insertion operator << for the output file stream
	//{
	//	for (int i = 0; i < registredVehiclesNo; i++) {
	//		outfile << v_List[i];
	//		outfile << "------------------------------------------\n";
	//	}
	//	return outfile;
	//}
	//ofstream & operator << (ofstream & oufile );
	friend ofstream &operator<<(ofstream&, const Inventory&);
	 //
	
	ofstream &operator<<( const Inventory&);
private:

	int capacity; // array size
	int registredVehiclesNo;
	NodeList<Vehicle> v_List; 
	/*Project 6: Refactoring is needed here. You need to replace the dynamic array with a Nodelist object.
      you must update all the affected methods in the entire project*/
};



#endif