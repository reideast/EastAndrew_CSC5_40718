/* 
    File:   main.cpp
    Author: Andrew Reid East
    Class: CSC-5 40718
    Created on January 14, 2015, 8:01 PM
    Purpose: output the velocity of sound across a range of temperatures
 */

//System Libraries
#include <iostream>
#include <cstdlib> //random number
#include <ctime> //seed random /w time()
using namespace std;

//User Libraries
//Global Constants
//Function Prototypes

//Execution Begins Here
int main(int argc, char** argv)
{
  //set up physical constants
  const float VELOCITY_BASE = 331.3f; //speed of sound in m/s at 0 Celsius
  const float VELOCITY_FACTOR = 0.61f; //multiply by temperature in Celsius to get speed of sound
  
  //working variables
  int tempLow = 0, tempHigh = 0, tempCurrent = 0;
  float velocityCurrent = 0.0f;
  
  //get user input
  cout << "Please enter the lower temperature in Celsius (whole numbers only): ";
  cin >> tempLow;
  cout << "Please enter the higher temperature in Celsius: ";
  cin >> tempHigh;
  
  if (tempLow > tempHigh)
  {
    cout << "The second temperature you entered is not higher." << endl;
    return 1;
  }
  
  tempCurrent = tempLow;
  while (tempCurrent <= tempHigh)
  {
    velocityCurrent = VELOCITY_BASE + VELOCITY_FACTOR * tempCurrent;
    cout << "At " << tempCurrent << " degrees Celsius the velocity of sound is " << velocityCurrent << " m/s" << endl;
    ++tempCurrent;
  }
  
  return 0;
}
