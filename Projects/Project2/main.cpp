

// https://msdn.microsoft.com/en-us/library/windows/desktop/ms685035(v=vs.85).aspx
// cls using WinAPI
// https://msdn.microsoft.com/en-us/library/windows/desktop/ms682022(v=vs.85).aspx



#include <windows.h> //for Windows-only implementation of keyboard input and CLS
#include <stdio.h> //for printf and stderr only, i think. remove those, then disable.


//<chrono> for simulation time keeping and <thread> for animation sleeping. I referenced: http://stackoverflow.com/questions/4184468/sleep-for-milliseconds
//chrono needs C++11. To configure g++ in NetBeans, right click on Project. Set Configuration -> Customize. Build -> C++ Compiler. C++ Standard == C++11.
#include <chrono>
#include <thread>



#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>

using namespace std;

bool WIN32_MODE = true;
const bool DEBUG_MODE = true;

//Game animation parameters: Tweak these to make the game run differently if there is a lot of flicker on your machine
const int ANIMATION_RATE = 50; //in milliseconds

bool isRunningInAWin32Console();
bool resizeConsole_win32(short cols, short rows);
bool getAKey(char& input);
bool cls();
bool win32_cls();

bool getMapFromFile(string filename, char *map, short maxX, short maxY);

void printStatus(string line1, string line2, short hp = 100, short mp = 100, short qtyPotion = 3);
void printMap(char *map, short maxX, short maxY);
void printControlScheme();

//Execution Begins Here
int main(int argc, char** argv)
{
  /*
  implement this:
  make WIN32_MODE flag into local
  
  player is a STRUCT
  monsters struct
  
  all NPC are in a (vector)
  player moves
  then iterate the vector for all NPC to take actions
  
  help screen
  inventory screen
  
  battle screen
  
  scroll map
  */
  
  short screenSizeMaxX = 100, screenSizeMaxY = 40;
  string mapFile = "gameMap.txt";
  
  const short NUM_STATUS = 4;
  string statusDictionary[4];
  const short STATUS_INIT = 0;
  statusDictionary[STATUS_INIT] = "You wake up in a dank room. You hear water slowly dripping, and you feel slimy";
  statusDictionary[STATUS_INIT + 1] = "mold on the floor.";
  const short STATUS_EXIT = 2;
  statusDictionary[STATUS_EXIT] = "Are you sure you want to exit?";
  statusDictionary[STATUS_EXIT + 1] = "(y/n)";
  
  //test for running in the proper console, and give a chance to quit if user wants
  WIN32_MODE = isRunningInAWin32Console();
  if (!WIN32_MODE)
  {
    cout << "You are not running this program in a Windows Command Prompt console. Input and\nanimation will be more primitive. It is recommended you quit and run the .exe\nfrom outside any IDE's.\nDo you want to continue anyway? (y/n) ";
    char choice = 0;
    cin >> choice;
    if (choice == 'n' || choice == 'N')
      return 1; //exits with Run Failed
  }
  else
  {
    if (!resizeConsole_win32(screenSizeMaxX, screenSizeMaxY)) //{cols, rows}
      cout << "Resizing the console window failed. This program will not work right with a console buffer smaller than 100 characters wide by 40 tall. Please use the settings to resize this console window before continuing." << endl;
  }
  
  short mapSizeMaxX = 80, mapSizeMaxY = 30;
  short x, y;
  short playerX = 20, playerY = 5;
  string statusLine1, statusLine2;
  
  char *map = new char[mapSizeMaxY * mapSizeMaxX];
  
  if (!getMapFromFile(mapFile, map, mapSizeMaxX, mapSizeMaxY))
  {
    cout << "Reading game map from file has failed." << endl;
    delete [] map;
    return 1;
  }
  
  // for (y = 0; y < mapSizeMaxY; ++y)
    // for (x = 0; x < mapSizeMaxX; ++x)
    // {
      // map[y * mapSizeMaxX + x] = ' ';
    // }
    
  char *playerPtr = &map[playerY * mapSizeMaxX + playerX];
  *playerPtr = 'Q';
  
  map[11 * mapSizeMaxX + 20] = 'W';
  
  short currStatusLine = STATUS_INIT;
  
  cls();
  printStatus(statusDictionary[currStatusLine], statusDictionary[currStatusLine + 1]);
  printMap(map, mapSizeMaxX, mapSizeMaxY);
  printControlScheme();
  
  char input = 0;
  bool isGameRunning = true;
  bool confirmExit = false;
  short confirmExitPrevStatus = 0;
  while (isGameRunning)
  {
    cls();
    printStatus(statusDictionary[currStatusLine], statusDictionary[currStatusLine + 1]);
    printMap(map, mapSizeMaxX, mapSizeMaxY);
    printControlScheme();
    
    if (getAKey(input))
    {
      if (WIN32_MODE) cout << endl;
      switch ((input >= 'a' && input <= 'z') ? (input - 'a' + 'A') : input) //to upper
      {
        case 'W':
          cout << "up" << endl;
          break;
        case 'A':
          cout << "left" << endl;
          break;
        case 'S':
          cout << "down" << endl;
          break;
        case 'D':
          cout << "right" << endl;
          break;
        case 'X':
          // cout << "Are you sure you want to exit? (y/n) ";
          confirmExit = true;
          confirmExitPrevStatus = currStatusLine;
          currStatusLine = STATUS_EXIT;
          // cin >> input;
          // if (getAKey(input))
          // {
            // if (input == 'y' || input == 'Y')
            // {
            // }
            // else if (input != 'n' && input != 'N')
            // {
              // cout << "Please input y or n." << endl;
            // }
          // }
          // else
            // cout << "Key input failed." << endl;
          break;
        case 'H':
          cout << "DEBUG: Display help here.";
          break;
        case 'Y':
        case 'N':
          if (confirmExit)
          {
            if (input == 'Y' || input == 'y')
            {
              cout << "Thank you for playing!" << endl;
              isGameRunning = false;
            }
            else // input == 'N'
            {
              confirmExit = false;
              currStatusLine = confirmExitPrevStatus;
            }
            break;
          }
        default:
          cout << "Keyboard input doesn't do anything" << endl;
      }
    }
    else
    {
      cout << "Key input failed." << endl;
    }
    
  }
  delete [] map;
  return 0;
  
  //test animation using cls()
  HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
  COORD coordScreen = { 0, 30 };
  //test cls:
  for (int i = 0; i < 77; ++i)
  {
    cls();
    SetConsoleCursorPosition(hStdout, coordScreen);
    cout << string(i, ' ') << ">=-" << endl;
    //wait for ANIMATION_RATE to establish proper frame rate
    this_thread::sleep_for(chrono::milliseconds(ANIMATION_RATE)); //reference: http://stackoverflow.com/questions/4184468/sleep-for-milliseconds
  }
 
  delete [] map;
  return 0; 
}



//******************************************************************************
//********************************   Graphics   ********************************
//******************************************************************************

void printStatus(string line1, string line2, short hp, short mp, short qtyPotion)
{
  cout << endl;
  cout << line1 << endl;
  cout << line2 << endl;
  cout << endl;
  cout << " [ HP: " << hp << " ]   [ MP: " << mp << " ]   [ Potions: " << qtyPotion << " ]" << endl;
}

void printMap(char *map, short sizeX, short sizeY)
{
  short x, y;
  for (y = 0; y < sizeY; ++y)
  {
    cout << setw(2) << y << " - ";
    for (x = 0; x < sizeX; ++x)
    {
      // if (!(x % 10))
        // cout << x;
      cout << map[y * sizeX + x];
    }
    cout << endl;
  }
}

void printControlScheme()
{
  cout << endl;
  cout << "Move:     (W)                (H)elp" << endl;
  cout << "       (A)(S)(D)             E(x)it" << endl;
  cout << "> ";
}







//******************************************************************************
//******************************   Setup Game   ********************************
//******************************************************************************

bool getMapFromFile(string filename, char *map, short maxX, short maxY)
{
  ifstream mapFile;
  mapFile.open(filename);
  if (mapFile.fail())
  {
    return false;
  }
  else
  {
    char c = 0;
    short i = 0;
    short x = 0, y = 0;
    short max = maxX * maxY;
    while (mapFile.get(c) && y <= maxY && x <= maxX)
    {
      // cout << static_cast<int>(c);
      // cout << c;
      // if (((i % maxX) + 1) == (maxX + 1)) //it is the last character in a line
      if (x == maxX)
      {
        // if (c != '\n') //it should be a CR
        if (c != 13) //it should be a CR
        {
          cout << c << endl;
          cout << i << endl;
          cout << "Improperly formatted Map file (" << filename << "). Line num " << (y + 1) << " is not the proper length." << endl;
          return false;
        }
        else //it is a newline, so ignore the character
        {
          if (mapFile.get(c))
          {
            if (c == 10) //LF
            {
              ++y;
              // cout << "Line " << y << " completed." << endl;
              x = 0;
            }
            else
            {
              cout << "There was no CR/LF at the end of the line." << endl;
              return false;
            }
          }
          else
          {
            cout << "Read failed." << endl;
            return false;
          }
        }
      }
      else
      {
        // map[i++] = c;
        map[y * maxX + x++] = c;
      }
    }
    if (y != maxY && x != maxX)
    {
      cout << "Map file  (" << filename << ") was not " << maxX << " wide by " << maxY << " tall. It was " << x << " by " << y << "." << endl;
      return false;
    }
  }
  mapFile.close();
  return true;
}

//tests if there is a win32 console running this program right now
bool isRunningInAWin32Console()
{
  // uses a function (from the Input Buffer Read method), then ignores the result
  DWORD fdwSaveOldMode;
  if (GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), &fdwSaveOldMode))
    return true; //console was found
  else
    return false; //console not found
}


//MSDN Method to resize the console window, increase buffer size
//https://msdn.microsoft.com/en-us/library/windows/desktop/ms686044(v=vs.85).aspx
bool resizeConsole_win32(short cols, short rows)
{
  HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
  COORD consoleSize = {cols, rows}; //{cols, rows}
  SMALL_RECT windowSize = {0, 0, static_cast<short>(cols - 1), static_cast<short>(rows - 1)};
  if(!SetConsoleScreenBufferSize(hStdout, consoleSize))
  {
    cout << "Setting the console size failed." << endl;
    return false;
  }
  if(!SetConsoleWindowInfo(hStdout, true, &windowSize ))
  {
    cout << "Setting the window size failed." << endl;
    return false;
  }
  return true;
}


//******************************************************************************
//***************************   Input / Output   *******************************
//******************************************************************************

//MSDN Method to read unbuffered console input
//https://msdn.microsoft.com/en-us/library/windows/desktop/ms685035(v=vs.85).aspx
bool getAKey(char& input)
{
  if(!WIN32_MODE)
  {
    cin >> input;
    return true;
  }
  else
  {
    cout.flush(); //necessary to display anything in cout before ReadConsoleInput starts polling?
    
    HANDLE hStdin;
    DWORD fdwSaveOldMode;
    DWORD cNumRead, fdwMode;
    const short BUFF_SIZE = 8; // original buffer size: const int BUFF_SIZE = 128;
    INPUT_RECORD irInBuf[BUFF_SIZE];
    
    // Get the standard input handle.
    hStdin = GetStdHandle(STD_INPUT_HANDLE);
    if (hStdin == INVALID_HANDLE_VALUE)
    {
      cout << "Error: GetStdHandle" << endl;
      // SetConsoleMode(hStdin, fdwSaveOldMode); // Restore input mode on exit.
      return false; //ExitProcess(0);
    }

    // Save the current input mode, to be restored on exit.
    if (!GetConsoleMode(hStdin, &fdwSaveOldMode))
    {
      cout << "Error: Getting the Console State Failed: No Console Found\nPlease run this program in a Windows Command Prompt console if you would like to enable input w/o pressing the enter key." << endl;
      // SetConsoleMode(hStdin, fdwSaveOldMode); // Restore input mode on exit.
      return false; //ExitProcess(0);
    }

    // change console mode to non-buffered input, allowing the Windows console to send input to C++ w/o waiting for the {Enter} key
    fdwMode = ENABLE_WINDOW_INPUT;
    if (!SetConsoleMode(hStdin, fdwMode))
    {
      cout << "Error: SetConsoleMode" << endl;
      SetConsoleMode(hStdin, fdwSaveOldMode); // Restore input mode on exit.
      return false; //ExitProcess(0);
    }

    // Loop to read and handle inputs until a valid character is read
    KEY_EVENT_RECORD keyEventRecord;
    int i = 0;
    bool isInputDone = false;
    while (!isInputDone)
    {
      // Wait for the events.
      if (!ReadConsoleInput(hStdin, irInBuf, BUFF_SIZE, &cNumRead))
      {
        cout << "Error: While waiting for ReadConsoleInput" << endl;
        SetConsoleMode(hStdin, fdwSaveOldMode); // Restore input mode on exit.
        return false; //ExitProcess(0);
      }
      
      if (DEBUG_MODE)
        if (cNumRead > 1)
          cout << "Warning: For this cycle, more than one (" << cNumRead << ") input event was read." << endl;

      // Check all events in the buffer (should be only one)
      for (i = 0; i < cNumRead; i++)
      {
        switch(irInBuf[i].EventType) //filter out all events that are not KEY_EVENT
        {
          case KEY_EVENT: // keyboard input
            keyEventRecord = irInBuf[i].Event.KeyEvent;
            //note: the hexadecimal "Virtual Key Codes" are from MSDN: https://msdn.microsoft.com/en-us/library/windows/desktop/dd375731(v=vs.85).aspx
            // ctrl+c: has to check dwControlKeyState using bit masks
            if (keyEventRecord.wVirtualKeyCode == 0x43 && (keyEventRecord.dwControlKeyState & 0x0008 || keyEventRecord.dwControlKeyState & 0x0004) && !keyEventRecord.bKeyDown)
            {
              cout << "Ctrl+C has been used to kill the process." << endl;
              ExitProcess(0); //from MSDN methods
            }
            if (!keyEventRecord.bKeyDown) //if the keyboard key has not been released yet, ignore it
            {
              isInputDone = true; //assume input is good. change it to false if input is actually bad
              
              //test if this is an allowed character
              if (keyEventRecord.wVirtualKeyCode >= '0' && keyEventRecord.wVirtualKeyCode <= '9')
                input = keyEventRecord.wVirtualKeyCode; // - 0x30 + '0'; //note: that was useless, 0x30=='0'==48. I need to check my ASCII table better!
              else if (keyEventRecord.wVirtualKeyCode >= 'A' && keyEventRecord.wVirtualKeyCode <= 'Z')
                input = keyEventRecord.wVirtualKeyCode;
              else if (keyEventRecord.wVirtualKeyCode == ' ') //space bar
                input = ' ';
              else if (keyEventRecord.wVirtualKeyCode == 13) //carriage return
                input = '\n';
              else
                isInputDone = false;
              if (isInputDone)
                printf("%c", input);
                // cout << input;
            }
          break;

          // Ignore all other potential event types
          case MOUSE_EVENT: // mouse input
          case WINDOW_BUFFER_SIZE_EVENT: // scrn buf. resizing
          case FOCUS_EVENT:  // disregard focus events
          case MENU_EVENT:   // disregard menu events
          default:
            break;
        }
      }
    }
    
    // Restore input mode on exit.
    SetConsoleMode(hStdin, fdwSaveOldMode);
    return true; //no errors
  }
}

//clear screen (will do it nicely using MSDN's console-handle method if enabled by WIN32_MODE
bool cls()
{
  if (WIN32_MODE)
    win32_cls();
  else //will create a lot of flicker! ew.
    for (int i = 0; i < 30; ++i)
      cout << endl;
}

//MSDN method of clearing the console by writing ' ' to every spot
//Reference: https://msdn.microsoft.com/en-us/library/windows/desktop/ms682022(v=vs.85).aspx
bool win32_cls()
{
  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  COORD coordScreen = {0, 0}; // top, left corner of the console
  DWORD cCharsWritten;
  CONSOLE_SCREEN_BUFFER_INFO csbi; 
  DWORD dwConSize;

  // Get the number of character cells in the current buffer. 
  if(!GetConsoleScreenBufferInfo(hConsole, &csbi))
    return false;
  dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

  // Fill the entire screen with blanks.
  if( !FillConsoleOutputCharacter(hConsole, (TCHAR) ' ', dwConSize, coordScreen, &cCharsWritten))
    return false;
  // Get the current text attribute.
  if(!GetConsoleScreenBufferInfo(hConsole, &csbi))
    return false;
  // Set the buffer's attributes accordingly.
  if(!FillConsoleOutputAttribute(hConsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten)) // Receive number of characters written
    return false;

  // Put the cursor at its home coordinates.
  SetConsoleCursorPosition(hConsole, coordScreen);
  
  return true;
}