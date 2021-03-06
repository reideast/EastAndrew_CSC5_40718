/* 
    File:   main.cpp
    Author: Andrew Reid East
    Class: CSC-5 40718
    Created on January 26, 2015, 11:16 PM
    Purpose: Windchill index calculator
 */

//System Libraries
#include <iostream>
#include <cmath>
using namespace std;

//User Libraries

//Global Constants

//Function Prototypes

//windchill: calculate windchill index W in terms of wind speed and temperature
float windchill(float velocity, float temp);
//Preconditions:
//  velocity is 0 or greater, in m/sec
//  temp is <= 10 degrees, in Celsius
//Postconditions:
//  returns W in degrees Celsius

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
    float velocity = 0.0f, temp = 0.0f, W = 0.0f;
    getInput("What is the wind speed in m/sec? (any positive number) ", velocity);
    getInput("What is the temperature in degrees Celsius? (any number 10 or lower) ", temp, -273.15f, 10); //minimum of absolute zero in degrees C
    
    W = windchill(velocity, temp);
    
    cout << "The windchill factor (W) is " << W << " in degrees Celsius." << endl;
    
    
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

float windchill(float v, float t)
{
  if (v < 0)
  {
    cout << "Wind speed cannot be a negative number." << endl;
    return 0;
  }
  else if (t > 10)
  {
    cout << "Temperature must be less than or equal to 10 degrees Celsius." << endl;
    return 0;
  }
  else
  {
    //typo fixed from book, according to wikipedia, the expoennt of both V terms should be 0.16
    float v_term = pow(v, 0.16); //only calculate expensive POW function once
    return 13.12f + 0.6215 * t - 11.37 * v_term + 0.3965 * t * v_term;
  }
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
