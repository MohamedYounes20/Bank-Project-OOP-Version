#include <iostream>
#include <iomanip>
#include "clsLogInScreen.h"



int main()

{
    while (true) //loop to solve circler ref --> (1- clsLogIn. will call 2- clsMainScreen. then Main want to call LogIn agin).(callstack)
        // after _LogOut() ends in clsMainScreen it will back to here, and clsLogInScreen will start agin .
    {
        if (!clsLogInScreen::ShowLogInScreen())
        {
            break;//to get out of program after geting locked
        }
    }
   
    system("pause>0");
    return 0;
}