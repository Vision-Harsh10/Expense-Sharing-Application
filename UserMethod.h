#include<bits/stdc++.h>
using namespace std;


class Observer{
public:
    virtual void notify(const string &message) = 0;
    virtual ~Observer() = default;
};

// User class implementing Observer
class User : public Observer{
public:
    static int nextUserId; // static variable to generate unique user IDs
    string userId;
    string name;
    string email;
    map<string, double> balances;  // UserId to balance mapping, (Positive = they owe you, Negative = you owe them)
public:

    // constructor
    User(const string &name, const string &email){
        this->userId = "User" + to_string(nextUserId++);
        this->name = name;
        this->email = email;
    }

    void update(const string & message) override {
        cout << "Notification for " << name << " (" << userId << "): " << message << endl;
    }

    void updateBalance(const string &otherUserId, double amount){
        balances[otherUserId] += amount;

        // Remove if balance becomes Zero

        if(abs(balances[otherUserId]) < 0.0001){ // because of floating point precision
            balances.erase(otherUserId);
        }   
    }

    double getTotalOwed(){ // ketne paise mujhe dene hai (-ve balance case)
        double total = 0.0;
        for(const auto &balance : balances){
            if(balance.second < 0){
                total += abs(balance.second); 
            }
        }
        return total;
    }

    double getTotalLent(){ // ketne paise mujhe lene hai (+ve balance case)
        double total = 0.0;
        for(const auto &balance : balances){
            if(balance.second > 0){
                total += balance.second; 
            }
        }
        return total;
    }
};
int User::nextUserId = 1; // Initialize static user ID counter




