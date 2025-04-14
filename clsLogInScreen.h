#pragma once
#include <iostream>
#include "clsScreen.h"
#include "Global.h"
#include "clsUser.h"
#include "clsMainScreen.h"
class clsLogInScreen : protected clsScreen
{
private:
    static bool _Login()
    {
        bool LoginFaild = false;
        short FaildLogInCount = 0;
        string Username, Password;
        do
        {
            
            if (LoginFaild)
            {
                FaildLogInCount++;
                cout << "\nInvlaid Username/Password!";
                cout << "\nYou have " << (3 - FaildLogInCount) << " Trial(s) to login.\n\n";
            }
            if (FaildLogInCount == 3)
            {
                cout << "\nYour are Locked after 3 faild trails \n\n";
                return false;
            }

            cout << "Enter Username? ";
            cin >> Username;

            cout << "Enter Password? ";
            cin >> Password;

            CurrentUser = clsUser::Find(Username, Password);

            LoginFaild = CurrentUser.IsEmpty();

        } while (LoginFaild);
        CurrentUser.RegisterLogIn();
        clsMainScreen::ShowMainMenue();
        return true;

    }
public:
	static bool ShowLogInScreen()
	{
        system("cls");
		_DrawScreenHeader("\t LogIn Screen");

        return _Login();
	}
};

