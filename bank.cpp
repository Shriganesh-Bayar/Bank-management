#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

string loadKey(const string &filename) {
    ifstream keyFile(filename);
    // created a file with this file name
    if (!keyFile) {
        cerr << "Error: Could not open key file!" << endl;
        return "";
    }
    string key, a;
    getline(keyFile, key);
    stringstream s;
    s << key;
    // stored in this format: encrypted_key : "the key is written here"
    s >> a >> a >> key;
    keyFile.close();
    return key;
}

void sleep() { for(int i = 0; i < 3e8; i++) {} }
string encrypt(string a) {
    string b = loadKey(".env");
    // used one method of encryption
    for(int i = 0; i < a.size(); i++) {
        int j = i % b.size();
        char c = ((a[i] - 'a') + (b[j] - 'a')) % 26 + 'a';
        a[i] = c;
    }
    return a;
}

class Account
{
private:
    string AccountNo, Password;
    int Balance;

public:
    Account() : AccountNo(""), Password(""), Balance(0) {}

    void setAccountNo(string id){
        AccountNo = id;
    }

    void setPassword(string pw){
        Password = encrypt(pw);
    }

    void setBalance(int balance){
        Balance = balance;
    }

    string getAccountNo(){
        return AccountNo;
    }

    string getPassword(){
        return Password;
    }

    int getBalance(){
        return Balance;
    }
};

void openAccount(Account user)
{
    system("clear");
    string id, pw;

    cout << "\tEnter Account No: ";
    cin >> id;
    user.setAccountNo(id);

    cout << "\tEnter A Strong Password: ";
    cin >> pw;
    user.setPassword(pw);

    user.setBalance(0);

    ofstream outfile("Account.txt", ios::app);// ios::app means it is in append mode
    if (!outfile)
        cout << "\tError: File Can't Open!\n";
    else
    {
        outfile << "\t" << user.getAccountNo() << " : " << user.getPassword() << " : " << user.getBalance() << "\n\n";
        cout << "\tAccount Created Successfuly!\n";
    }
    outfile.close();
    sleep();
}

void addCash()
{ 
    system("clear");
    string id;
    cout << "\tEnter Account No: ";
    cin >> id;

    ifstream infile("Account.txt");
    ofstream outfile("tempAccount.txt");
    if (!infile || !outfile)
        cout << "\tError: File Can't Open!\n";

    string line;
    bool found = false;

    while (getline(infile, line))
    {
        stringstream ss;
        ss << line;
        string userID, userPW;
        int userBalance;
        char delimiter;

        ss >> userID >> delimiter >> userPW >> delimiter >> userBalance;
        if (id == userID)
        {
            found = true;
            int cash;
            cout << "\tEnter Cash: ";
            cin >> cash;

            int newBalance = userBalance + cash;
            userBalance = newBalance;
            outfile << "\t" << userID << " : " << userPW << " : " << userBalance << '\n';
            cout << "\tNew Balance Is: " << userBalance << '\n';
        }
        else
            outfile << line << '\n';
    }
    if (!found)
        cout << "\tEnter Valid Account No!\n";
    outfile.close();
    infile.close();
    remove("Account.txt");
    rename("tempAccount.txt", "Account.txt");
    sleep();
}

void withdraw()
{
    string id, pw;
    cout << "\tEnter Account No: ";
    cin >> id;

    cout << "\tEnter Password: ";
    cin >> pw;
    ifstream infile("Account.txt");
    ofstream outfile("tempAccount.txt");
    if (!infile || !outfile)
        cout << "\tError: File Can't Open!\n";

    string line;
    bool found = false;
    while (getline(infile, line))
    {
        stringstream ss;
        ss << line;
        string userID, userPW;
        int userBalance;
        char delimiter;

        ss >> userID >> delimiter >> userPW >> delimiter >> userBalance;
        if (id == userID && userPW == encrypt(pw))
        {
            found = true;
            int cash;
            cout << "\tEnter Cash: ";
            cin >> cash;
            if (cash <= userBalance)
            {
                int newBalance = userBalance - cash;
                userBalance = newBalance;
                outfile << "\t" << userID << " : " << userPW << " : " << userBalance << '\n';
                cout << "\tTransaction Was Successful!\n";
                cout << "\tRemaining Balance: " << userBalance << '\n';
            }
            else
            {
                cout << "\tLow Balance!\n";
                outfile << "\t" << userID << " : " << userPW << " : " << userBalance << '\n';
            }
        }
        else
            outfile << line << '\n';
        for(int i = 0; i < 5e8; i++) {}
    }
    if (!found)
        cout << "\tInvalid Account No or Password!\n";
    outfile.close();
    infile.close();
    remove("Account.txt");
    rename("tempAccount.txt", "Account.txt");
    sleep();
}

int main()
{
    Account user;
    bool exit = false;
    while (!exit)
    {
        system("clear");
        int val;
        cout << "\tWelcome To Bank Account Management System\n";
        cout << "\t*****************************************\n";
        cout << "\t1.Open New Account.\n";
        cout << "\t2.Add Cash.\n";
        cout << "\t3.Withdraw Cash.\n";
        cout << "\t4.Exit.\n";
        cout << "\tEnter Your Choice: ";
        cin >> val;

        switch (val)
        {
        case 1: openAccount(user);
            break;
        case 2: addCash();
            break;
        case 3: withdraw();
            break;
        case 4: system("clear");
            exit = true;
            cout << "\tGood Luck!\n";
            sleep();
            break;
        
        default: cout << "Invalid Choice\nTry Again\n";
            break;
        }
        sleep();
    }
}
