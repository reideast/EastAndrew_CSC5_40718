

// https://msdn.microsoft.com/en-us/library/windows/desktop/ms685035(v=vs.85).aspx
// cls using WinAPI
// https://msdn.microsoft.com/en-us/library/windows/desktop/ms682022(v=vs.85).aspx



#include <windows.h> //for Windows-only implementation of keyboard input and CLS
// #include <stdio.h> //for printf and stderr only, i think. remove those, then disable.


//<chrono> for simulation time keeping and <thread> for animation sleeping. I referenced: http://stackoverflow.com/questions/4184468/sleep-for-milliseconds
//chrono needs C++11. To configure g++ in NetBeans, right click on Project. Set Configuration -> Customize. Build -> C++ Compiler. C++ Standard == C++11.
#include <chrono>
#include <thread>


#include <iostream>
#include <string>

using namespace std;

bool WIN32_MODE = true;
const bool DEBUG_MODE = true;

//Game animation parameters: Tweak these to make the game run differently if there is a lot of flicker on your machine
const int ANIMATION_RATE = 50; //in milliseconds

bool isRunningInAWin32Console();
bool getAKey(char& input);
bool cls();
bool win32_cls();



//Execution Begins Here
int main(int argc, char** argv)
{
  cout << WIN32_MODE;
  WIN32_MODE = isRunningInAWin32Console();
  cout << WIN32_MODE;
  
  cout << "0x30=" << 0x30 << " '0'=" << static_cast<int>('0') << endl;
  cout << "0x41=" << 0x41 << " 'A'=" << static_cast<int>('A') << endl;
  cout << "0x5A=" << 0x5A << " 'Z'=" << static_cast<int>('Z') << endl;
  
  char input = 0;
  
  if (getAKey(input))
    cout << "You typed in: '" << input << "'"  << endl;
  return 0;
  
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
 
  return 0; 
}

//tests if there is a win32 console running this program right now
bool isRunningInAWin32Console()
{
  // uses a function (from the Input Buffer Read method), then ignores the result
  HANDLE hStdin;
  DWORD fdwSaveOldMode;
  if (GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), &fdwSaveOldMode))
    return true; //console was found
  else
    return false; //console not found
}

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
            // if (keyEventRecord.wVirtualKeyCode == 0x43 && (keyEventRecord.dwControlKeyState & 0x0008 || keyEventRecord.dwControlKeyState & 0x0004) && !keyEventRecord.bKeyDown)
              // ErrorExit("");
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
              // if (DEBUG_MODE) cout << "DEBUG: string to return: \"" << input << "\"";
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
