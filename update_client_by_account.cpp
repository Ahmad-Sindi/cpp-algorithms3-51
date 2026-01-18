#include <iostream>
#include <vector>
#include <iomanip>
#include <string>
#include <fstream>
using namespace std;

/*
 * Algorithm Challenge – Level 3 | #51 Update Client by Account Number
 *
 * This program demonstrates how to update an existing client record stored in a text file using a safe file-based update strategy.
 *
 * Engineering Workflow:
 *   - Load all client records from the file
 *   - Search for the client by Account Number
 *   - Display current client information
 *   - Modify the selected client data
 *   - Rewrite the entire file with updated data
 *
 * This approach ensures data consistency when working with text files.
 */

const string ClientFileName1 = "Clients.txt";

// Structure that represents client data
struct ClientStructData
{
    string AccountNumber;
    string PinCode;
    string Name;
    string Phon;
    double AccountBalance;
    bool MarkForDelete = false; // Reserved for future use
};

// Splits a string based on a delimiter
vector<string> SplitString(string S1, string Delim)
{
    vector<string> VecString;
    short Position = 0;
    string Token;

    while ((Position = S1.find(Delim)) != string::npos)
    {
        Token = S1.substr(0, Position);

        if (Token != "")
            VecString.push_back(Token);

        S1.erase(0, Position + Delim.length());
    }

    if (S1 != "")
        VecString.push_back(S1);

    return VecString;
}

// Converts a line of text into a client structure
ClientStructData ConvertLineToRecord(string Line, string Seperator = "#//#")
{
    ClientStructData Client;
    vector<string> ClientData = SplitString(Line, Seperator);

    Client.AccountNumber  = ClientData[0];
    Client.PinCode        = ClientData[1];
    Client.Name           = ClientData[2];
    Client.Phon           = ClientData[3];
    Client.AccountBalance = stod(ClientData[4]);

    return Client;
}

// Converts a client structure into a text line
string ConvertRecordToLine(ClientStructData Client, string Seperator = "#//#")
{
    string ClientRecord = "";

    ClientRecord += Client.AccountNumber + Seperator;
    ClientRecord += Client.PinCode + Seperator;
    ClientRecord += Client.Name + Seperator;
    ClientRecord += Client.Phon + Seperator;
    ClientRecord += to_string(Client.AccountBalance);

    return ClientRecord;
}

// Loads all clients from file
vector<ClientStructData> LoadClientsDataFromFile(string FileName)
{
    vector<ClientStructData> VecClients;
    fstream FileClients;

    FileClients.open(FileName, ios::in);

    if (FileClients.is_open())
    {
        string Line;
        ClientStructData Client;

        while (getline(FileClients, Line))
        {
            Client = ConvertLineToRecord(Line);
            VecClients.push_back(Client);
        }

        FileClients.close();
    }

    return VecClients;
}

// Prints a single client card
void PrintClientCard(ClientStructData Client)
{
    cout << "\nClient Details\n";
    cout << "----------------------\n";
    cout << "Account Number : " << Client.AccountNumber << endl;
    cout << "Pin Code       : " << Client.PinCode << endl;
    cout << "Full Name      : " << Client.Name << endl;
    cout << "Phone Number   : " << Client.Phon << endl;
    cout << "Account Balance: " << Client.AccountBalance << endl;
}

// Searches for a client by account number
bool FindClientByAccountNumber(string AccountNumber,
                               vector<ClientStructData> vClients,
                               ClientStructData& Client)
{
    for (ClientStructData C : vClients)
    {
        if (C.AccountNumber == AccountNumber)
        {
            Client = C;
            return true;
        }
    }
    return false;
}

// Reads updated client data from the user
ClientStructData ChangeClientRecord(string AccountNumber)
{
    ClientStructData Client;
    Client.AccountNumber = AccountNumber;

    cout << "Enter New Pin Code       : ";
    getline(cin >> ws, Client.PinCode);

    cout << "Enter New Full Name      : ";
    getline(cin, Client.Name);

    cout << "Enter New Phone Number   : ";
    getline(cin, Client.Phon);

    cout << "Enter New Account Balance: ";
    cin >> Client.AccountBalance;

    return Client;
}

// Saves all clients back to file
void SaveClientsDataToFile(string FileName,
                           vector<ClientStructData> vClients)
{
    fstream FileClients;
    FileClients.open(FileName, ios::out); // Overwrite mode

    if (FileClients.is_open())
    {
        for (ClientStructData Client : vClients)
        {
            if (!Client.MarkForDelete)
            {
                FileClients << ConvertRecordToLine(Client) << endl;
            }
        }
        FileClients.close();
    }
}

// Updates a client by account number
bool UpdateClientByAccountNumber(string AccountNumber,
                                 vector<ClientStructData>& vClients)
{
    ClientStructData Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        PrintClientCard(Client);

        cout << "\nAre you sure you want to update this client? (Y/N): ";
        cin >> Answer;

        if (toupper(Answer) == 'Y')
        {
            for (ClientStructData& C : vClients)
            {
                if (C.AccountNumber == AccountNumber)
                {
                    C = ChangeClientRecord(AccountNumber);
                    break;
                }
            }

            SaveClientsDataToFile(ClientFileName1, vClients);
            cout << "\nClient updated successfully.\n";
            return true;
        }
    }
    else
    {
        cout << "\nClient with Account Number (" 
             << AccountNumber << ") not found.\n";
    }

    return false;
}

// Reads account number from user
string ReadClientAccountNumber()
{
    string AccountNumber;
    cout << "Enter Account Number: ";
    cin >> AccountNumber;
    return AccountNumber;
}

int main()
{
    vector<ClientStructData> vClients =
        LoadClientsDataFromFile(ClientFileName1);

    string AccountNumber = ReadClientAccountNumber();
    UpdateClientByAccountNumber(AccountNumber, vClients);

    return 0;
}
