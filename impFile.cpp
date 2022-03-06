
#include <string>
#include <iostream>
#include <fstream>
#include "transportation.h"
#include "NodeList.h"
using namespace std;

/****************************Class Vehicle Method Implementations***************************************/
Vehicle::Vehicle() { 
	/*default constructor
	//fields are initialized with default data.
	//for the vehicle object, an empty dynamic array is allocated.
	*/
	vehicleID = -1;
	make = "NA";
	model = "NA";
	seats = 0;
	available = true;
	extarFeatures = 0;
};

Vehicle::Vehicle(int _vehicleID_, string _make_, string _model_, int _seats_,
	bool _available_, int _extarFeatures_, NodeList<string> _otherFeaturesList_) {
	/*overloaded constructor
	//fields are initialized with data sent via the constructor.
	//the array of features is sent fro outsite as a pointer.
	*/
	vehicleID = _vehicleID_;
	make = _make_;
	model = _model_;
	seats = _seats_;
	available = _available_;
	extarFeatures = _extarFeatures_;
	//otherFeaturesList = _otherFeaturesList_;
	NodeList<string>::Iterator curr = _otherFeaturesList_.begin();
	while (curr != _otherFeaturesList_.end()) {
		otherFeaturesList.insertBack(*curr);
		++curr;
	}
};

//-------------
// if (v1>v2) 
bool Vehicle::operator> (const Vehicle& v) { 
	/* 
	// overloading the greater than operator >. number of seats is used for
	//comparing the vehcle. If equal, then the number of extra features is used to break the ties.
	*/
		return ((seats > v.seats) || ((seats == v.seats) && (extarFeatures > v.extarFeatures)));
}

bool Vehicle::operator< (const Vehicle& v) { // used seats then extrafeatures as keys
		/*
	// overloading the less than operator <. number of seats is used for
	//comparing the vehcle. If equal, then the number of extra features is used to break the ties.
	*/
	return ((seats < v.seats) || ((seats == v.seats) && (extarFeatures < v.extarFeatures)));
}


bool Vehicle::operator==(const Vehicle& v) // uses seats then extrafeatures as keys to compare between Vehicle objects
{
	/*
// overloading the equality operator ==. Number of seats and extra features are used 
//for determining if two vehicles are equal.
*/
	return (this->seats == v.seats && this->extarFeatures == v.extarFeatures);
}

bool Vehicle::operator<=(const Vehicle& v) // seats is the key
{
	/* overloading operator <= */
	return ((this->seats == v.seats && this->extarFeatures == v.extarFeatures) || ((seats < v.seats) || ((seats == v.seats) && (extarFeatures < v.extarFeatures))));
}
bool Vehicle::operator>=(const Vehicle& v) // seats is the key
{	/* overloading operator >= */
	return ((this->seats == v.seats && this->extarFeatures == v.extarFeatures) || ((seats > v.seats) || ((seats == v.seats) && (extarFeatures > v.extarFeatures))));

}

bool Vehicle::operator!=(const Vehicle& v) // seats is the key
{
	/* overloading the  operator != not equal*/
	return (!((this->seats == v.seats) && (this->extarFeatures == v.extarFeatures)));
}


//Rule of Three:
//destructor
Vehicle::~Vehicle() { 
	NodeList <string>::Iterator curr = otherFeaturesList.begin();
	while (curr != otherFeaturesList.end()) {
		otherFeaturesList.eraseFront();
		curr=otherFeaturesList.begin();
	}
};
// copy constructor	
Vehicle::Vehicle(const Vehicle& org) {  	
	vehicleID = org.vehicleID;
	make = org.make;
	model = org.model;
	seats = org.seats;
	available = org.available;
	extarFeatures = org.extarFeatures;
	NodeList <string>::Iterator curr1 = otherFeaturesList.begin();
	while (curr1 != otherFeaturesList.end()) {
		otherFeaturesList.eraseFront();
		curr1 = otherFeaturesList.begin();
	}
	NodeList <string>::Iterator curr2 = org.otherFeaturesList.begin();
	while (curr2 != org.otherFeaturesList.end()) {
		otherFeaturesList.insertBack(*curr2);
		++curr2;
	}
	//for (int i = 0; i < extarFeatures; ++i)
	//{
	//	otherFeaturesList = org.otherFeaturesList;
	//}
}
//assignment operator overloading
Vehicle Vehicle::operator=(const Vehicle& rhs) {
	if (this != &rhs)
	{
		NodeList <string>::Iterator curr1 = otherFeaturesList.begin();
		while (curr1 != otherFeaturesList.end()) {
			otherFeaturesList.eraseFront();
			curr1 = otherFeaturesList.begin();
		}
		NodeList <string>::Iterator curr2 = rhs.otherFeaturesList.begin();
		while (curr2 != rhs.otherFeaturesList.end()) {
			otherFeaturesList.insertBack(*curr2);
			++curr2;
		}
		vehicleID = rhs.vehicleID;
		make = rhs.make;
		model = rhs.model;
		seats = rhs.seats;
		available = rhs.available;
		extarFeatures = rhs.extarFeatures;

	//	for (int i = 0; i < extarFeatures; ++i)
	//		otherFeaturesList = rhs.otherFeaturesList;

	}
	return *this;
}

/******** Friend Functions to the Vehicle***********/
ostream & operator<<(ostream &out, const Vehicle &objV)
// overleaoding the insertion operator <<
{
	cout << "------------------------------------------\n";
	cout <<"ID: "<< objV.vehicleID << endl;
	cout << "Make: " << objV.make << endl;
	cout << "Model: " << objV.model << endl;
	cout << "Vumber of seats: "<<objV.seats << endl;
	if (objV.available)
		cout << "Availability: available" << endl;
	else
		cout << "Availability: Not available" << endl;
	cout << "Extra Features["<< objV.extarFeatures <<"]: " << endl;
	if (objV.extarFeatures < 1)
		cout << "Basic trim" << endl;
	else {
		cout << "[";
		NodeList <string>::Iterator curr = objV.otherFeaturesList.begin();
		while (curr != objV.otherFeaturesList.end()) {
			cout << *curr << ", ";
			++curr;
		}
		cout << "]" << endl;
	}
	return out;
}
ofstream & operator<<(ofstream &out, const Vehicle &objV)
// overleaoding the insertion operator << for the output file stream
{
	out << objV.vehicleID << endl;
	out << objV.make << endl;
	out << objV.model << endl;
	out << objV.seats << endl;
	out << objV.available << endl;
	out << objV.extarFeatures << endl;
	NodeList <string>::Iterator curr = objV.otherFeaturesList.begin();
	while (curr != objV.otherFeaturesList.end()) {
		out << *curr << endl;
		++curr;
	}
	
	return out;
}



/****************************Class Inventory Method Implementations***************************************/
//default constructor
Inventory::Inventory() {
	registredVehiclesNo = 0;
}

//constructor II
Inventory::Inventory(int _capacity_) {
	registredVehiclesNo = 0;
	//	registredVehiclesNo = capacity;
}
Inventory::~Inventory() //destructor
{
	NodeList <Vehicle>::Iterator curr = v_List.begin();
	while (curr != v_List.end()) {
		v_List.eraseFront();
		curr = v_List.begin();
	}
}
//copy constructor
Inventory::Inventory(const Inventory& obj) {
	NodeList<Vehicle>::Iterator curr1 = obj.v_List.begin();
	while (curr1 != v_List.end()) {
		v_List.eraseFront();
		curr1 = v_List.begin();
	}
	registredVehiclesNo = obj.registredVehiclesNo;
	NodeList<Vehicle>::Iterator curr2 = obj.v_List.begin();
	while (curr2 != obj.v_List.end()) {
		v_List.insertBack(*curr2);
		++curr2;
	}
}

//assignment overloading.
Inventory Inventory::operator=(const Inventory& rhs) {
	if (this != &rhs) {
		capacity = rhs.capacity;
		registredVehiclesNo = rhs.registredVehiclesNo;
		NodeList<Vehicle>::Iterator curr = rhs.v_List.begin();
		while (curr != rhs.v_List.end()) {
			v_List.insertBack(*curr);
			++curr;
		}
	}
	return *this;
}
//
bool Inventory::push_Back(Vehicle  ve) {
	/*inserting vehilce object to the inventory list*/
	/*Project 6: improve it by extending the underlining list, expands as needed.*/
	/*Project 6: make sure that it is consistent with the new data structure (NodeList) you used for this class.*/
	try {
		v_List.insertBack(ve);
		++registredVehiclesNo;
	}
	catch(...){
		return false;
	}
}// end of push_back
//
void Inventory::sortList() {
	/*sorts the inventory list in decreasing order. See the operator > overloading for the class Vehicle*/
	NodeList<Vehicle>::Iterator first = v_List.begin();
	NodeList<Vehicle>::Iterator second = ++first;
	for (first; first != v_List.end(); ++first)
		for (second; second != v_List.end(); ++second)
			if (*second > *first) {
				Vehicle strTmpTask = *second;
				*second = *first;
				*first = strTmpTask;
			}
}
void Inventory::printList(NodeList<Vehicle>::Iterator curr) {
	if (curr != v_List.end()) {
		cout << *curr;
		cout << "------------------------------------------\n";
		printList(++curr);
	}
	else
	{
		return;
}
}
void Inventory::printResevedList(NodeList<Vehicle>::Iterator curr) {  // print list of the reserved vehicles
	/*Project 6: Refactoring is needed here. You need to reimplement this method in recursive way.*/
	if (curr != v_List.end()) {
		if (!(*curr).status()) {
			cout << *curr << endl;

		}
		printResevedList(++curr);
	}
	else
		return;
}
void Inventory::printAvailableList(NodeList<Vehicle>::Iterator curr) {// print list of the available vehicles
	/*Project 6: Refactoring is needed here. You need to reimplement this method in recursive way.*/
	if (curr != v_List.end()) {
		if ((*curr).status()) {
			cout << *curr << endl;

		}
		printAvailableList(++curr);
	}
	else
		return;
}
bool Inventory::found(NodeList<Vehicle>::Iterator curr, int seatsNo, int& count) {
	// print list of the available vehicles that has AT LEAST seatsNo seats.
	//Pre-conditions: seatsNo is a positive value > 0
	/*Project 6: Refactoring is needed here. You need to reimplement this method in recursive way.*/
	if(curr != v_List.end()){
		if ((*curr).status() && (*curr).getSeats() >= seatsNo) {
			cout << *curr << endl;
			++count;
		}
		found(++curr, seatsNo, count);
	}
	if (count > 0)
		return true;
	else
		return false;
}
bool Inventory::reserveVehicle(int _vehicleID_) {
	NodeList<Vehicle>::Iterator curr = v_List.begin();
	try {
		while (curr != v_List.end()) {
			if ((*curr).getID() == _vehicleID_ && (*curr).status()) {  //found and available			
				(*curr).reserve();
				return true;
			}
			else ++curr;
		}
		throw 100;
	}
	catch (...) {
		cout << "Vehicle ID not found among list of cars that are currently available. Please try again." << endl;
		return false;
	}
}
bool Inventory::returnVehicle(int _vehicleID_) {
	NodeList<Vehicle>::Iterator curr = v_List.begin();
	try {
		while (curr != v_List.end()) {
			if ((*curr).getID() == _vehicleID_ && !(*curr).status()) {  //found and available			
				(*curr).unReserve();
				return true;
			}
			else ++curr;
		}
		throw 100;
	}
	catch (...) {
		cout << "Vehicle ID not found among list of cars that have been reserved. Please try again." << endl;
		return false;
	}
}
int Inventory::checkID(int _vehicleID_) {
	NodeList<Vehicle>::Iterator curr = v_List.begin();
	/*Project 6: Refactoring is needed here. You need to reimplement this method in recursive way.*/
	for (int i = 0; i < registredVehiclesNo; ++i)
		if ((*curr).getID() == _vehicleID_)
			return i;
	return -1;

}


bool Inventory::saveToFile(string fileName) {
	/*this method prints the list data in a text file (received as a parameter) */
	//pre-conditions: fileName is a valid file name.  something.txt
	ofstream outFile;
	NodeList<Vehicle>::Iterator curr = v_List.begin();
	outFile.open(fileName);
	if (outFile.fail()) return false;
	outFile << v_List.size() << endl;
	while (curr != v_List.end()) { // updating the database
		outFile << *curr;
		++curr;
	}
	outFile.close();
	return true;

}


/************** Freind functions to class Inventory ******************/


ofstream & operator<<(ofstream &outfile, const Inventory &objV)
// overleaoding the insertion operator << for the output file stream
{
	NodeList<Vehicle>::Iterator curr = objV.v_List.begin();
	while (curr != objV.v_List.end()) {
				outfile << *curr;
				++curr;
	}
			return outfile;
}

