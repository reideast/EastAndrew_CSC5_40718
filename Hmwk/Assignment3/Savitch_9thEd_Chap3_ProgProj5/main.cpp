/* 
    File:   main.cpp
    Author: Andrew Reid East
    Class: CSC-5 40718
    Created on January 19, 2015, 8:32 PM
    Purpose: Calculate interest on a loan as the principle decreases
 */

//System Libraries
#include <iostream>
#include <iomanip>
using namespace std;

//User Libraries
//Global Constants
//Function Prototypes

//Execution Begins Here
int main(int argc, char** argv)
{
  cout << fixed << setprecision(2);

  //main program, loops until user asks to quit (copied from my Savitch_9thEd_Chap3_ProgProj1)
  bool isUserFinished = false; //controls main loop
  char quit = 0;
  bool isInputValid = false; //controls individual input loops (ie, loop until input is right)
  while (!isUserFinished)
  {
    const int numPayments = 20;
    
    float totalLoan = 0.0f, principle = 0.0f, monthlyPayment = 0.0f, interest = 0.0f, monthlyInterest = 0.0f, thisMonthsInterest = 0.0, totalInterestPaid = 0.0f;
    //input loop; until input is verified as valid (copied from my Savitch_9thEd_Chap3_ProgProj1)
    isInputValid = false;
    while (!isInputValid)
    {
      cout << "What is the initial amount of the loan? $";
      cin >> totalLoan;
      if (totalLoan < 0) //don't accept negative numbers
        cout << "The amount of the loan cannot be negative. Please input again." << endl;
      else
        isInputValid = true;
    }

    isInputValid = false;
    while (!isInputValid)
    {
      cout << "What is the yearly interest percentage (ex. 14.5 for 14.5%)? ";
      cin >> interest;
      if (interest < 0) //don't accept negative numbers
        cout << "The interest cannot be negative. Please input again." << endl;
      else
        isInputValid = true;
    }
    
    //make initial calculations before loop
    monthlyInterest = interest / 100 / 12; //12 months in a year, converted to a decimal
    //cout << "DEBUG: monthlyInterest=" << monthlyInterest << endl;
    monthlyPayment = totalLoan / numPayments; //problem defines monthly payments as 1/20th of total amount
    cout << "For a loan of $" << totalLoan << " your monthly payment will be $" << monthlyPayment << "." << endl;
    
    //set up loop variables
    principle = totalLoan;
    int countMonths = 1;
    //loop until the loan has been paid off
    while (principle > 0)
    {
      thisMonthsInterest = principle * monthlyInterest; //calculate interest paid this month
      totalInterestPaid += thisMonthsInterest; //save that interest paid in the total
      principle -= (monthlyPayment - thisMonthsInterest); //reduce the principle
      
      //output message depending on if the loan amount has just been paid off
      if (principle > 0)
        cout << "Month #" << countMonths++ << ": Principle is now $" << principle << ", and interest paid this month was $" << thisMonthsInterest << endl;
      else
        cout << "Month #" << countMonths << ": Congrats! You paid $" << (monthlyPayment + principle) << " to pay off the loan, and interest paid this month was $" << thisMonthsInterest << endl;
    }
    
    cout << endl;
    cout << "The total interest paid was $" << totalInterestPaid << "." << endl;
    
    float years = countMonths / 12.0;
    //cout << "DEBUG: years=" << years << endl;
    cout << "The annualized interest rate is therefore " << (((totalInterestPaid / years) / totalLoan) * 100) << "%." << endl;
    
    
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
