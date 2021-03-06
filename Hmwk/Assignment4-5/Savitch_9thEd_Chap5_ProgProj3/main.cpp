/* 
    File:   main.cpp
    Author: Andrew Reid East
    Class: CSC-5 40718
    Created on January 26, 2015, 10:34 PM
    Purpose: In 12-hour notation, figure out time after waiting a certain number of minutes
 */

//System Libraries
#include <iostream>
#include <iomanip>
using namespace std;

//User Libraries

//Global Constants

//Function Prototypes

//getTime12Hour: read a 12-hour time from <iostream>
void getTime12Hour(int& hour, int& min, char& am);
//Postconditions:
//  hour will be set to 0-23
//  min will be set to 0-59
//  am will be set to 'A' or 'P'

//outputTime12Hour: puts a 12 hour time to <iostream>
void outputTime12Hour(int hour, int min, char am);
//Preconditions:
// hour is 1-12, min is 0-59, am is 'A' or 'P'

//convertTime24to12Hour: change a 24-hour time to 12 hour time with AM/PM
void convertTime24to12Hour(int hour24, int min24, int& hour12, int& min12, char& am);
//Preconditions
//  hour24 is 0-23
//  min24 is 0-59
//Postconditions:
//  hour12 will be set to 0-23
//  min12 will be set to 0-59
//  am will be set to 'A' or 'P'

//convertTime12to24Hour: change a 12-hour time to 24
void convertTime12to24Hour(int hour12, int min12, char am, int& hour24, int& min24);
//Preconditions
//  hour12 will be 0-23
//  min12 will be 0-59
//  am will be 'A' or 'P'
//Postconditions:
//  hour24 is 0-23
//  min24 is 0-59

//getTime24Hour: read a 24-hour time from <iostream>
void getTime24Hour(int& hour, int& min);
//Postconditions:
//  hour will be set to 0-23
//  minute will be set to 0-59

//outputTime24Hour: puts a 24 hour time to <iostream>
void outputTime24Hour(int hour, int min);
//Preconditions:
// hour is 0-23, min is 0-59

//waitTime: advance time for a certain number of hours and minutes to wait
void waitTime(int startHours, int startMin, int waitHours, int waitMin, int& futureHours, int& futureMin);
//note: Problem description does not specify what happens when you wait until the next day, simply asks for what time it will be
//      So, I will just drop any date data and show what time it will be

//GetInput: Get input from <iostream> between a minimum and maximum value
//  (From my Savitch_9thEd_Chap4_ProgProj1)
void getInput(string prompt, int& input, int min = 0, int max = ~(1 << (sizeof(int) * 8) - 1));
void getInput(string prompt, float& input, float min = 0, float max = 1.0e38f);
//Preconditions:
//  prompt contains something to display through cout.
//  input is an initialized variable.
//  min/max contains the range desired (inclusive)
//    if not included, min/max will be zero to the largest integer on this system (calculated a bit shift based on the number of bytes in an int, then inversion (only works on two's complement based systems!))
//    for a float, a hard-coded value of 1.0e38f is chosen (based on windows 7 x64 g++ machine)
//Postconditions:
//  input will contain a valid value within the range

//Execution Begins Here
int main(int argc, char** argv)
{
  //main program, loops until user asks to quit (copied from my Savitch_9thEd_Chap3_ProgProj1)
  bool isUserFinished = false; //controls main loop
  char quit = 0;
  while (!isUserFinished)
  {
    int startHours = 0, startMin = 0, waitHours = 0, waitMin = 0, futureHours = 0, futureMin = 0;
    char startAM = 0, futureAM = 0;
    
    cout << "What is the current time?" << endl;
    getTime12Hour(startHours, startMin, startAM);
    convertTime12to24Hour(startHours, startMin, startAM, startHours, startMin);
    
    getInput("How many hours to wait? ", waitHours); //unrestricted amount to time to wait, as show in project description
    getInput("How many minutes to wait? ", waitMin);
    
    waitTime(startHours, startMin, waitHours, waitMin, futureHours, futureMin);
    
    convertTime24to12Hour(startHours, startMin, startHours, startMin, startAM);
    cout << "The time you have started at is ";
    outputTime12Hour(startHours, startMin, startAM);
    cout << endl;
    
    convertTime24to12Hour(futureHours, futureMin, futureHours, futureMin, futureAM);
    cout << "The time you have waited until is ";
    outputTime12Hour(futureHours, futureMin, futureAM);
    cout << endl;
    
    //determine if the user would like to repeat the calculation (copied from my Savitch_9thEd_Chap3_ProgProj1)
    cout << endl;
    cout << "Would you like to do more calculations? (N to quit) ";
    cin >> quit;
    if (quit == 'N' || quit == 'n')
      isUserFinished = true;
    else
      cout << endl; //a blank line for readability
  }
  return 0;
}



void outputTime12Hour(int hour, int min, char am)
{
  cout << hour << ":";
  cout << setfill('0') << setw(2) << min << " ";
  if (am == 'A')
    cout << "AM";
  else if (am == 'P')
    cout << "PM";
  else
    cout << "AM/PM variable was not either 'A' or 'P'";
}

//convertTime24to12Hour: change a 24-hour time to 12 hour time with AM/PM
void convertTime24to12Hour(int hour24, int min24, int& hour12, int& min12, char& am)
{
  if (hour24 < 12)
    am = 'A';
  else
    am = 'P';
  
  hour12 = hour24 % 12; // gives 0-11
  if (hour12 == 0)
    hour12 += 12;
  
  min12 = min24;
}

//convertTime12to24Hour: change a 24-hour time to 12 hour time with AM/PM
void convertTime12to24Hour(int hour12, int min12, char am, int& hour24, int& min24)
{
  hour24 = hour12 % 12; //changes 1-12 into 0-11
  if (am == 'P')
    hour24 += 12; // changes 0-11/0-11 to 0-23
  
  min24 = min12;
}

void getTime12Hour(int& hour, int& min, char& am)
{
  cout << "Please input time in 12-hour notation." << endl;
  getInput("Input hour as a number from 1-12: ", hour, 1, 12);
  getInput("Input minutes as a number from 0-59: ", min, 0, 59);
  bool isInputValid = false; //controls input loop (ie, loop until input is right)
  while (!isInputValid)
  {
    cout << "Please input \"A\" or \"P\" for AM or PM: ";
    cin >> am;
    if (am == 'A' || am == 'a')
    {
      am = 'A';
      isInputValid = true;
    }
    else if (am == 'P' || am == 'p')
    {
      am = 'P';
      isInputValid = true;
    }
    else
    {
      cout << "You must answer A or P." << endl;
    }
  }

}

void getTime24Hour(int& hour, int& min)
{
  cout << "Please input time in 24-hour notation." << endl;
  getInput("Input hour as a number from 0-23: ", hour, 0, 23);
  getInput("Input minutes as a number from 0-59: ", min, 0, 59);
}

void outputTime24Hour(int hour, int min)
{
    cout << setfill('0') << setw(2) << hour << ":" << setw(2) << min;
}

void waitTime(int startHours, int startMin, int waitHours, int waitMin, int& futureHours, int& futureMin)
{
  futureHours = startHours + waitHours;
  futureMin = startMin + waitMin;
  futureHours += futureMin / 60; //if futureMin is now over 60, add extra hours into futureHours
  futureMin %= 60; //remove those extra hours from futureMin
  
  futureHours %= 24; //if we went over to the next day, disregard extra hours and go back to 0-23
}



void getInput(string prompt, int& input, int min, int max)
{
  bool isInputValid = false; //controls input loop (ie, loop until input is right)
  while (!isInputValid)
  {
    cout << prompt;
    cin >> input;
    if (input < min)
      cout << "The input cannot be less than " << min << ". Please input again." << endl;
    else if (input > max)
      cout << "The input cannot be greater than " << max << ". Please input again." << endl;
    else
      isInputValid = true;
  }
}

void getInput(string prompt, float& input, float min, float max)
{
  bool isInputValid = false; //controls input loop (ie, loop until input is right)
  while (!isInputValid)
  {
    cout << prompt;
    cin >> input;
    if (input < min)
      cout << "The input cannot be less than " << min << ". Please input again." << endl;
    else if (input > max)
      cout << "The input cannot be greater than " << max << ". Please input again." << endl;
    else
      isInputValid = true;
  }
}
