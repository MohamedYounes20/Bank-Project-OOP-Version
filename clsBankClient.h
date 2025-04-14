#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "clsPerson.h"
#include "clsString.h"
using namespace std;
class clsBankClients : public clsPerson 
{
private:
	enum enMode {EmptyMode=0 , UpdateMode=1, AddNewMode=2};
	enMode _Mode;

	string _AccountNumber;
	string _PinCode;
	double _AccountBalance;
	bool _MarkedForDelete = false;

	//Abstraction - Hide This Function From User because he has nothing to do with it -
	static clsBankClients _ConvertLineToClientObject(string Line, string seperator="#//#")//Abstraction - Hide This Function From User because he has nothing to do with it -
	{
		vector<string> vClientData; 
		vClientData = clsString::Split(Line, seperator);

		return clsBankClients(enMode::UpdateMode, vClientData[0], vClientData[1], vClientData[2],
			           vClientData[3], vClientData[4], vClientData[5], stod(vClientData[6]));
	}

	static clsBankClients _GetEmptyClientObject()
	{
		return clsBankClients(enMode::EmptyMode, "", "", "", "", "", "", 0);
	}

	static string _ConverClientObjectToLine(clsBankClients Client, string Seperator = "#//#")
	{

		string stClientRecord = "";
		stClientRecord += Client.FirstName + Seperator;
		stClientRecord += Client.LastName + Seperator;
		stClientRecord += Client.Email + Seperator;
		stClientRecord += Client.Phone + Seperator;
		stClientRecord += Client.AccountNumber() + Seperator;
		stClientRecord += Client.PinCode + Seperator;
		stClientRecord += to_string(Client.AccountBalance);

		return stClientRecord;

	}

	static void _SaveCleintsDataToFile(vector <clsBankClients> vClients)
	{

		fstream MyFile;
		MyFile.open("Clients.txt", ios::out);//overwrite

		string DataLine;

		if (MyFile.is_open())
		{

			for (clsBankClients C : vClients)
			{
				DataLine = _ConverClientObjectToLine(C);
				MyFile << DataLine << endl;

			}

			MyFile.close();

		}

	}

	

	static  vector <clsBankClients> _LoadClientsDataFromFile()
	{

		vector <clsBankClients> vClients;

		fstream MyFile;
		MyFile.open("Clients.txt", ios::in);//read Mode

		if (MyFile.is_open())
		{

			string Line;


			while (getline(MyFile, Line))
			{

				clsBankClients Client = _ConvertLineToClientObject(Line);

				vClients.push_back(Client);
			}

			MyFile.close();

		}

		return vClients;

	}

	void _Update()
	{
		vector <clsBankClients> _vClients;
		_vClients = _LoadClientsDataFromFile();

		for (clsBankClients& C : _vClients)
		{
			if (C.AccountNumber() == AccountNumber())
			{
				C = *this; // the object that I'm Working At 
				//object I'm Working At  to  vector  then vector to files
				break;
			}

		}

		_SaveCleintsDataToFile(_vClients);

	}

	void _AddNew()
	{

		_AddDataLineToFile(_ConverClientObjectToLine(*this)); //the object on which I Stand
	}

	void _AddDataLineToFile(string  stDataLine)
	{
		fstream MyFile;
		MyFile.open("Clients.txt", ios::out | ios::app);

		if (MyFile.is_open())
		{

			MyFile << stDataLine << endl;

			MyFile.close();
		}

	}
	struct stTrnsferLogRecord;

	static stTrnsferLogRecord _ConvertTransferLogLineToRecord(string Line, string Seperator = "#//#")
	{
		stTrnsferLogRecord TrnsferLogRecord;

		vector <string> vTrnsferLogRecordLine = clsString::Split(Line, Seperator);
		TrnsferLogRecord.DateTime = vTrnsferLogRecordLine[0];
		TrnsferLogRecord.SourceAccountNumber = vTrnsferLogRecordLine[1];
		TrnsferLogRecord.DestinationAccountNumber = vTrnsferLogRecordLine[2];
		TrnsferLogRecord.Amount = stod(vTrnsferLogRecordLine[3]);
		TrnsferLogRecord.srcBalanceAfter = stod(vTrnsferLogRecordLine[4]);
		TrnsferLogRecord.destBalanceAfter = stod(vTrnsferLogRecordLine[5]);
		TrnsferLogRecord.UserName = vTrnsferLogRecordLine[6];

		return TrnsferLogRecord;

	}
	string _PrepareTransferLogRecord(float Amount, clsBankClients DestinationClient,
		string UserName, string Seperator = "#//#")
	{
		string TransferLogRecord = "";
		TransferLogRecord += clsDate::GetSystemDateTimeString() + Seperator;
		TransferLogRecord += AccountNumber() + Seperator;
		TransferLogRecord += DestinationClient.AccountNumber() + Seperator;
		TransferLogRecord += to_string(Amount) + Seperator;
		TransferLogRecord += to_string(AccountBalance) + Seperator;
		TransferLogRecord += to_string(DestinationClient.AccountBalance) + Seperator;
		TransferLogRecord += UserName;
		return TransferLogRecord;
	}

	void _RegisterTransferLog(float Amount, clsBankClients DestinationClient, string UserName)
	{

		string stDataLine = _PrepareTransferLogRecord(Amount, DestinationClient, UserName);

		fstream MyFile;
		MyFile.open("TransferLog.txt", ios::out | ios::app);

		if (MyFile.is_open())
		{

			MyFile << stDataLine << endl;

			MyFile.close();
		}

	}

public:

	struct stTrnsferLogRecord
	{
		string DateTime;
		string SourceAccountNumber;
		string DestinationAccountNumber;
		float Amount;
		float srcBalanceAfter;
		float destBalanceAfter;
		string UserName;

	};


	clsBankClients(enMode Mode, string FirstName, string LastName, string Email, string Phone, string AccountNumber, string PinCode, double AccountBalance)
		: clsPerson(FirstName, LastName, Email, Phone)
	{
		_Mode = Mode;
		_AccountNumber = AccountNumber;
		_PinCode = PinCode;
		_AccountBalance = AccountBalance;
	}

	bool isEmpty()
	{
		return(_Mode == enMode::EmptyMode);
	}

	string AccountNumber()//Read Only (AccountNumber)
	{
		return _AccountNumber;
	}

	void SetPinCode(string PinCode)//property Set (PinCode)
	{
		_PinCode = PinCode;
	}
	string GetPinCode()//property Get (PinCode)
	{
		return _PinCode;
	}
	__declspec(property(get = GetPinCode, put = SetPinCode)) string PinCode;

	void SetAccountBalance(double AccountBalance)
	{
		_AccountBalance = AccountBalance;
	}
	double GetAccountBalance()
	{
		return _AccountBalance;
	}
	__declspec(property(get = GetAccountBalance, put = SetAccountBalance)) double AccountBalance;

	//Find Client Data By his Acc Num - return client constructor - 
	static clsBankClients Find(string AccountNumber)
	{
		fstream myFile;
		myFile.open("Clients.txt", ios::in);
		if (myFile.is_open())
		{
			string Line;
			
			while (getline(myFile, Line))
			{
				clsBankClients Client = _ConvertLineToClientObject(Line);
				if (Client.AccountNumber() == AccountNumber)
				{
					myFile.close();
					return Client;
				}
			}
			myFile.close();

		}
		return _GetEmptyClientObject();
	}

	static clsBankClients Find(string AccountNumber, string PinCode)//for log in
	{
		fstream myFile;
		myFile.open("Clients.txt", ios::in);
		if (myFile.is_open())
		{
			string Line;

			while (getline(myFile, Line))
			{
				clsBankClients Client = _ConvertLineToClientObject(Line);
				if (Client.AccountNumber() == AccountNumber && Client.PinCode == PinCode)
				{
					myFile.close();
					return Client;
				}
			}
			myFile.close();

		}
		return _GetEmptyClientObject();
	}

	enum enSaveResults { svFaildEmptyObject = 0, svSucceeded = 1, svFaildAccountNumberExists = 2 };


	enSaveResults Save()
	{

		switch (_Mode)
		{
		case enMode::EmptyMode:
		{
			if (isEmpty())
			{

				return enSaveResults::svFaildEmptyObject;

			}

		}

		case enMode::UpdateMode:
		{


			_Update();

			return enSaveResults::svSucceeded;

			break;
		}

		case enMode::AddNewMode:
		{
			//This will add new record to file or database
			if (clsBankClients::isClientExist(_AccountNumber))
			{
				return enSaveResults::svFaildAccountNumberExists;
			}
			else
			{
				_AddNew();

				//We need to set the mode to update after add new
				_Mode = enMode::UpdateMode;
				return enSaveResults::svSucceeded;
			}

			break;
		}
		}

	}

	bool Delete()
	{
		vector <clsBankClients> _vClients;
		_vClients = _LoadClientsDataFromFile();

		for (clsBankClients& C : _vClients)
		{
			if (C.AccountNumber() == _AccountNumber)
			{
				C._MarkedForDelete = true;
				break;
			}

		}

		_SaveCleintsDataToFile(_vClients);// put all clients from this vector to file except the mark for delete true

		*this = _GetEmptyClientObject();//this object that I stand on make it empty 

		return true;

	}


	static bool isClientExist(string AccountNumber)
	{
		clsBankClients Client1 = clsBankClients::Find(AccountNumber);
		return (!Client1.isEmpty());
	}

	static clsBankClients GetAddNewClientObject(string AccountNumber)
	{
		return clsBankClients(enMode::AddNewMode, "", "", "", "", AccountNumber, "", 0);
	}
	void Deposit(double Amount)
	{
		_AccountBalance += Amount;
		Save();
	}
	bool WithDraw(double Amount)
	{
		if (Amount > _AccountBalance)
		{
			return false;
		}
		else
		{
			_AccountBalance -= Amount ;
			Save();
		}
	}
	void Print()
	{
		cout << "\nClient Card:";
		cout << "\n___________________";
		cout << "\nFirstName   : " << FirstName;
		cout << "\nLastName    : " << LastName;
		cout << "\nFull Name   : " << FullName();
		cout << "\nEmail       : " << Email;
		cout << "\nPhone       : " << Phone;
		cout << "\nAcc. Number : " << _AccountNumber;
	    cout << "\nPassword    : " << _PinCode;
		cout << "\nBalance     : " << _AccountBalance;
		cout << "\n___________________\n";
	}


	static vector <clsBankClients> GetClientsList()
	{
		return _LoadClientsDataFromFile();
	}

	static float GetTotalBalances()
	{
		vector <clsBankClients> vClients = clsBankClients::GetClientsList();

		double TotalBalances = 0;

		for (clsBankClients Client : vClients)
		{

			TotalBalances += Client.AccountBalance;
		}

		return TotalBalances;

	}
	bool Transfer(float Amount, clsBankClients& DestinationClient, string UserName)
	{
		if (Amount > AccountBalance)
		{
			return false;
		}

		WithDraw(Amount);
		DestinationClient.Deposit(Amount);
		_RegisterTransferLog(Amount, DestinationClient, UserName);
		return true;
	}

	
	static  vector <stTrnsferLogRecord> GetTransfersLogList()
	{
		vector <stTrnsferLogRecord> vTransferLogRecord;

		fstream MyFile;
		MyFile.open("TransferLog.txt", ios::in);//read Mode

		if (MyFile.is_open())
		{

			string Line;

			stTrnsferLogRecord TransferRecord;

			while (getline(MyFile, Line))
			{

				TransferRecord = _ConvertTransferLogLineToRecord(Line);

				vTransferLogRecord.push_back(TransferRecord);

			}

			MyFile.close();

		}

		return vTransferLogRecord;

	}


};

