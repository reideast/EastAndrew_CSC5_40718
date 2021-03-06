/* 
    File:   main.cpp
    Author: Andrew Reid East
    Class: CSC-5 40718
    Created on January 25, 2015, 11:01 PM
    Purpose: Determine change from between 1 and 99 cents
 */

//System Libraries
#include <iostream>
using namespace std;

//User Libraries

//Global Constants

//Function Prototypes

//Note: function description copied from book!
//compute_coins: set the number of coins of denomination coin_value that can be given in change
void compute_coins(int coin_value, int& num, int& amount_left);
//Precondition: 0 < coin_value < 100; 0 <= amount_left < 100.
//Postcondition: num has been set equal to the maximum number
//of coins of denomination coin_value cents that can be obtained
//from amount_left. Additionally, amount_left has been decreased
//by the value of the coins, that is, decreased by
//num * coin_value.

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
    int moneyTotal = 0, moneyRemaining = 0, numQuar = 0, numDimes = 0, numPennies = 0;
    getInput("Please input a number of cents from 0 to 99: ", moneyTotal, 0, 99);
    moneyRemaining = moneyTotal;
    
    compute_coins(25, numQuar, moneyRemaining);
    compute_coins(10, numDimes, moneyRemaining);
    compute_coins(1, numPennies, moneyRemaining);
    
    if (moneyTotal != 0)
      cout << "Error in the algorithm: money did not end up at 0 cents." << endl;
    
    cout << moneyTotal << " cents can be given as" << endl;
    cout << numQuar << " quarters(s) " << numDimes << " dimes(s) and " << numPennies << " penny(pennies)" << endl;
    
    
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

//changing variable names to my preferred style
void compute_coins(int coinValue, int& numCoins, int& moneyRemaining)
{
  numCoins = moneyRemaining / coinValue;
  //moneyRemaining -= numCoins * coinValue;
  moneyRemaining %= coinValue; //changed to mod since problem definition asked for it
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
