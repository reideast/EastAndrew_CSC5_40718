/* 
    File:   main.cpp
    Author: Andrew Reid East
    Class: CSC-5 40718
    Created on January 19, 2015, 10:06 PM
    Purpose: Approximate e^x
 */

//System Libraries
#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;

//User Libraries
//Global Constants
//Function Prototypes

//Execution Begins Here
int main(int argc, char** argv)
{
  cout << setprecision(8);
  
  //main program, loops until user asks to quit (copied from my Savitch_9thEd_Chap3_ProgProj1)
  bool isUserFinished = false; //controls main loop
  char quit = 0;
  bool isInputValid = false; //controls individual input loops (ie, loop until input is right)
  while (!isUserFinished)
  {
    float x = 0.0f;
    //input loop; until input is verified as valid (copied from my Savitch_9thEd_Chap3_ProgProj1)
    isInputValid = false;
    while (!isInputValid)
    {
      cout << "For e^x, what is x? ";
      cin >> x;
      if (x < 0) //don't accept negative numbers
        cout << "The exponent must be zero or greater. Please input again." << endl;
      else
        isInputValid = true;
    }
    
    cout << "The standard value of e^" << x << " = " << exp(x) << endl;

    // ****** naive way, which overflows pretty much any data type I have attempted:
    // approx = 1.0;
    // double prevFactorial = 1; //as recommend by problem to prevent "integer overflow"
    // for (int n = 1; n <= 100; ++n)
    // {
      // cout << endl << "x^n=" << pow(x, n) << endl;
      // cout << "fact(" << n << ")=" << prevFactorial * n << endl;
      // approx = approx + (pow(x, n) / (prevFactorial *= n));
      // cout << n << "=" << approx << " ";
      // if (!(n % 10))
        // cout << endl;
    // }
    
    //non-naive calculation needed to prevent overflow:
    /*
      currTerm -> x^(n-1)*x / (n-1)!*n
      prev     ->  x^(n-1) / (n-1)!
      currTerm -> (x^(n-1) / (n-1!)) * (x / n)
      currTerm == (prev) * (x / n)
    */
    double approx = 0.0; //double variable type chosen purposefully to maintain precision for at least 8 digits displayed
    double prevSequenceItem = 1.0;
    for (int n = 1; n <= 100; ++n)
    {
      approx = approx + prevSequenceItem;
      prevSequenceItem = prevSequenceItem * (x / n);
      cout << n << "=" << approx << " ";
      if (!(n % 10))
        cout << endl;
    }
    
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
