#include "stackImp.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;



int main()
{
    Stack<string> lineStack;
    Stack<string> wordStack;
    ifstream lineInput;
    string tmpLine;
    string tmpWord;
    int oddCheck = 3;
    lineInput.open("encryptedMessage.txt");

    while (!lineInput.eof()) {
        getline(lineInput, tmpLine); //Grabs line and puts it in temporary string
        lineStack.push(tmpLine); //Pushes line to the line stack
    }
    while (!lineStack.empty()){ //Loop while there are still lines in the stack
        for (int i = 0; i < tmpLine.length(); i++) { //Loops through one line
            if ((tmpLine[i] == ' ') || (tmpLine[i] == '\0')) { //Checks if current character in line is blank
                ++i;
                if (oddCheck % 2 == 1) { //checks if word is even or odd
                    reverse(tmpWord.begin(), tmpWord.end()); //reverses characters in word
                }
                oddCheck++; //iterates oddCheck
                wordStack.push(tmpWord); //Pushes word to the word stack
                cout << wordStack.atTop() << " ";
                tmpWord.clear();
            }
            tmpWord += tmpLine[i]; //If current character is not blank, it is added to tmpWord
            if (i == tmpLine.size()-1) { //Checking if this word is the final word
                if (oddCheck % 2 == 1) { //Making sure that the final word of the line is reversed if needed
                    reverse(tmpWord.begin(), tmpWord.end());
                }
                cout << tmpWord << "." << endl; //Ending line with final word and period
            }
        }
            lineStack.pop(); //removes line from stack
            if (!lineStack.empty()) { 
                tmpLine = lineStack.atTop();
                tmpWord.clear();
            }
            oddCheck = 3; //resets check
    }
    lineInput.close();
    system("PAUSE");
    return 0;
}
