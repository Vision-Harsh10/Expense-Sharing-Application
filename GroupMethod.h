#include <bits/stdc++.h>
using namespace std;

// Group class --> Concrete Observable
class Group {
private:
    User* getUserByuserId(string userId) {
        User* user = nullptr;

        for(User *member : members) {
            if(member->userId == userId) {
                user = member;
            }
        }
        return user;
    }
    
public:
    static int nextGroupId;
    string groupId;
    string name;
    vector<User*> members; //observers
    map<string, Expense*> groupExpenses; // Group's own expense book
    map<string, map<string, double>> groupBalances; // memberId -> {otherMemberId -> balance}
    
    Group(const string& name) {
        this->groupId = "group" + std::to_string(++nextGroupId);
        this->name = name;
    }
    
    ~Group() {
        // Clean up group's expenses
        for (auto& pair : groupExpenses) {
            delete pair.second;
        }
    }
    
    void addMember(User* user) {
        members.push_back(user);

        // Initialize balance map for new member
        groupBalances[user->userId] = map<string, double>();
        cout << user->name << " added to group " << name << endl;
    }
    
    bool removeMember(const string& userId) {    
        // Check if user can be removed or not
        if(!canUserLeaveGroup(userId)) {
            cout <<"\nUser not allowed to leave group without clearing expenses" << endl;
            return false;
        }
        
        // Remove from observers
        for (User *user : members) {
            if (user->userId == userId) {
                members.erase(remove(members.begin(), members.end(), user),members.end());
                break;
            }
        }
        
        // Remove from group balances
        groupBalances.erase(userId);
        
        // Remove this user from other members' balance maps
        for (auto& memberBalance : groupBalances) {
            memberBalance.second.erase(userId);
        }
        return true;
    }
    
    void notifyMembers(const string& message) {
        for (Observer* observer : members) {
            observer->update(message);
        }
    }

    bool isMember(const string& userId) {
        return groupBalances.find(userId) != groupBalances.end();
    }
    
    // Update balance within group
    void updateGroupBalance(const string& fromUserId, const string& toUserId, double amount) {
        groupBalances[fromUserId][toUserId] += amount;
        groupBalances[toUserId][fromUserId] -= amount;
        
        // Remove if balance becomes zero
        if (abs(groupBalances[fromUserId][toUserId]) < 0.01) {
            groupBalances[fromUserId].erase(toUserId);
        }
        if (abs(groupBalances[toUserId][fromUserId]) < 0.01) {
            groupBalances[toUserId].erase(fromUserId);
        }
    }
    
    // Check if user can leave group.
    bool canUserLeaveGroup(const string& userId) {
        if (!isMember(userId)) {
            throw runtime_error("user is not a part of this group");
        };
        
        // Check if user has any outstanding balance with other group members
        map<string, double> userBalanceSheet = groupBalances[userId];
        for (auto& balance : userBalanceSheet) {
            if (abs(balance.second) > 0.01) {
                return false; // Has outstanding balance
            }
        }
        return true;
    }
    
    // Get user's balance within this group
    map<string, double> getUserGroupBalances(const string& userId) {
        if (!isMember(userId)) {
            throw runtime_error("user is not a part of this group");
        };
        return groupBalances[userId];
    }
    
    // Add expense to this group
    bool addExpense(string& description, double amount, string& paidByUserId,
                   vector<string>& involvedUsers, SplitType splitType, 
                   const vector<double>& splitValues = {}) {
        
        if (!isMember(paidByUserId)) {
            throw runtime_error("user is not a part of this group");
        }
        
        // Validate that all involved users are group members
        for (const string& userId : involvedUsers) {
            if (!isMember(userId)) {
                throw runtime_error("involvedUsers are not a part of this group");
            }
        }
        
        // Generate splits using strategy pattern
        vector<Split> splits = SplitFactory::getSplitStrategy(splitType)
                                ->calculateSplit(amount, involvedUsers, splitValues);
        
        // Create expense in group's own expense book
        Expense* expense = new Expense(description, amount, paidByUserId, splits, groupId);
        groupExpenses[expense->expenseId] = expense;
        
        // Update group balances
        for (Split& split : splits) {
            if (split.userId != paidByUserId) {
                // Person who paid gets positive balance, person who owes gets negative
                updateGroupBalance(paidByUserId, split.userId, split.amount);
            }
        }
        
        cout << endl << "=========== Sending Notifications ===================="<<endl;
        string paidByName = getUserByuserId(paidByUserId)->name;
        notifyMembers("New expense added: " + description + " (Rs " + to_string(amount) + ")");
        
        // Printing console message-------
        cout << endl << "=========== Expense Message ===================="<<endl;
        cout << "Expense added to " << name << ": " << description << " (Rs " << amount 
             << ") paid by " << paidByName <<" and involved people are : "<< endl;
        if(!splitValues.empty()) {
            for(int i=0; i<splitValues.size(); i++) {
                cout << getUserByuserId(involvedUsers[i])->name << " : " << splitValues[i] << endl;
            }
        } 
        else {
            for(string user : involvedUsers) {
                cout << getUserByuserId(user)->name << ", ";
            }
            cout << endl << "Will be Paid Equally" << endl;
        }    
        //-----------------------------------
        
        return true;
    }
    
    bool settlePayment(string& fromUserId, string& toUserId, double amount) {
        // Validate that both users are group members
        if (!isMember(fromUserId) || !isMember(toUserId)) {
            cout << "user is not a part of this group" << endl;
            return false;
        }
        
        // Update group balances
        updateGroupBalance(fromUserId, toUserId, amount);
        
        // Get user names for display
        string fromName = getUserByuserId(fromUserId)->name;
        string toName = getUserByuserId(toUserId)->name;
        
        // Notify group members
        notifyMembers("Settlement: " + fromName + " paid " + toName + " Rs " + to_string(amount));
        
        cout << "Settlement in " << name << ": " << fromName << " settled Rs " 
             << amount << " with " << toName << endl;
        
        return true;
    }
    
    void showGroupBalances() {
        cout << "\n=== Group Balances for " << name << " ===" << endl;
        
        for (const auto& pair : groupBalances) {
            string memberId = pair.first;
            string memberName = getUserByuserId(memberId)->name;

            cout << memberName << "'s balances in group:" << endl;
            
            auto userBalances = pair.second;
            if (userBalances.empty()) {
                cout << "  No outstanding balances" << endl;
            } 
            else {
                for (const auto& userBalance : userBalances) {
                    string otherMemberUserId = userBalance.first;
                    string otherName = getUserByuserId(otherMemberUserId)->name;
                    
                    double balance = userBalance.second;
                    if (balance > 0) {
                        cout << "  " << otherName << " owes: Rs " << fixed << setprecision(2) << balance << endl;
                    } else {
                        cout << "  Owes " << otherName << ": Rs " << fixed << setprecision(2) << abs(balance) << endl;
                    }
                }
            }
        }
    }

    void simplifyGroupDebts() {
        map<string, map<string, double>> simplifiedBalances = DebtSimplifier::simplifyDebts(groupBalances);
        groupBalances = simplifiedBalances;
    
        cout << "\nDebts have been simplified for group: " << name << endl;
    }
};
int Group::nextGroupId = 0;