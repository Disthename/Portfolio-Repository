// VehileManagment_TransportationSystem.cpp 
//Dr. Alnaeli, University of Wisconsin, 2020
//note: system("cls"); is ro run the system command cls that clears the screen

#include "transportation.h"

#include "NodeList.h"
#include <string>
#include <iostream>
#include <fstream>
using namespace std;
string menu() {

	string message;
	message = "0. Clear Screen..\n1. Print All of the registered  vehicles.\n2. Print list of the available vehicles.";
	message = message + "\n3.List of the reserved vehicles.\n4. Check availability.\n5.Reserve a vehicle.\n6. return a vehicle.\n7. Print All of the registered vehicles to a text file.\n8. Exit.\nYour Selection --> ";
	return message;
}
bool readingDataFromDBfile(Inventory& myInventory,  ifstream& inFile, int recordsInFile);

int main()
{
	int zero = 0;
	//////testing Vehicle class. You are welcome to reuse it.
	//if (testingVehicle())
	//	cout << "Class Vehicle Passed all the available tests" << endl;

	////cout << "Loading the data .";
	ifstream inFile;
	ofstream outFile;
	inFile.open("vehicles.txt");
	int recordsInFile = 0;
	inFile >> recordsInFile; // reading number of the records in the file.
	//inFile.ignore(1000, '\n');  // ignore the \n
	Inventory myInventory(recordsInFile); // capacity is set to numer of the records in the file.
	if (!readingDataFromDBfile(myInventory, inFile, recordsInFile)) {
		cout << "" << endl;
		exit(1); //terminate// exceptions
	}
	cout << endl;
	myInventory.sortList();
	myInventory.printList(myInventory.getList().begin()); //testing
	cout << endl;
	cout << menu();
	int userEntry = 0;
	cin >> userEntry;
	int seatsNo = 0;
	//system("cls") is used for clearing the console screen. cls is a Dos command
	
		do {
			try {
				switch (userEntry) {
				case 0: system("cls"); break;
				case 1: system("cls");  myInventory.printList(myInventory.getList().begin());  break;
				case 2: system("cls");  myInventory.printAvailableList(myInventory.getAvaList().begin()); break;
				case 3: system("cls"); myInventory.printResevedList(myInventory.getResList().begin()); break;
				case 4: {system("cls");
					cout << "Enter number of seats? "; cin >> seatsNo;
					//cout<<"GPS [Y/N]?"
					if (!myInventory.found(myInventory.getAvaList().begin(), seatsNo, zero)) cout << "Not available\n";
				}break;
				case 5: {system("cls"); cout << "Please Enter the Vehicle ID: ";
					int tmpVehicleID;
					cin >> tmpVehicleID;
					if (myInventory.reserveVehicle(tmpVehicleID))
					{
						if (!myInventory.saveToFile("vehicles.txt"))
							cout << endl;
					}
					else
						cout << endl;
				}
					  break;
				case 6: {system("cls"); cout << "Please Enter the Vehicle ID: ";
					int tmpVehicleID;
					cin >> tmpVehicleID;
					if (myInventory.returnVehicle(tmpVehicleID))
					{
						if (!myInventory.saveToFile("vehicles.txt"))
							cout << endl;
					}
					else
						cout << endl;
				}
					  break;

				case 7: {
					string fileName = "";
					cout << "Please enter a file name without an extension (Ex: myFile) -> "; cin >> fileName;
					ofstream outFile;
					fileName.append(".txt");
					outFile.open(fileName);
					if (outFile.fail()) { cout << "\nSystem was not able to create a file\n"; break; }
					outFile << myInventory;
					outFile.close();
					cout << "\nData sent to file specified\n"; } break;
				case 8: system("cls"); break;
				default: { system("cls");
					throw(100);
					break;
				}
				}// end switch
				if (userEntry == 8) break; // exit
				cout << menu();
				cin >> userEntry;
			}
			catch (...) {
				cout << "Invalid Input- Try again" << endl;
				cout << menu();
				cin >> userEntry;
			}
		} while (userEntry != 8);
	
	cout << "Ending the program..... Done!" << endl;
	system("PAUSE");
	return 0;
}

bool readingDataFromDBfile(Inventory& myInventory, ifstream& inFile, int recordsInFile) {
	/*reading the inventory informaton (cars that are registered) from the file to the inventory list (data structure.*/
	try {
		int vehicleID;
		string make;
		string model;
		int seats;
		bool available;
		int extarFeatures;
		NodeList<string> otherFeaturesList;
		for (int i = 0; i < recordsInFile; i++) { // reading the vehcile information from the file
			//cout << " .";
			inFile >> vehicleID;
			inFile.ignore(1000, '\n');
			getline(inFile, make);
			getline(inFile, model);
			inFile >> seats;
			inFile >> available;
			inFile >> extarFeatures;
			if (extarFeatures > 0)
				inFile.ignore(1000, '\n');
			
			for (int k = 0; k < extarFeatures; k++) {
				string tmp;
				getline(inFile, tmp);
				otherFeaturesList.insertBack(tmp);
			}

			Vehicle tempVehicle(vehicleID, make, model, seats, available, extarFeatures, otherFeaturesList);
			myInventory.push_Back(tempVehicle);
			NodeList<string>::Iterator curr = otherFeaturesList.begin();
			while (curr != otherFeaturesList.end()) {
				otherFeaturesList.eraseFront();
				curr = otherFeaturesList.begin();
			}
		}
		inFile.close();

	}
	catch (...) {
		return false;
	}

	return  true;
}