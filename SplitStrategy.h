#include<bits/stdc++.h>
using namespace std;
enum class SplitType{
    EQUAL, // Equal split
    EXACT, // Unequal split
    PERCENTAGE // How much percentage each user will pay
};

class Split{
public:
    string userId;
    double amount;

    Split(const string &userId, double amount){
        this->userId = userId;
        this->amount = amount;
    }

};


// Strategy Pattern - Split Strategy interface 
class SplitStrategy{
public:                                 //Amount to be split, list of users, optional values based on split type
    virtual vector<Split> calculateSplit(double totalAmount, const vector<string> &userIds,
                                    const vector<double> &values= {}) = 0;
    virtual ~SplitStrategy() = default;
    }; 


\

// Equal Split Strategy
class EqualSplitStrategy : public SplitStrategy{ 
public:
    vector<Split> calculateSplit(double totalAmount, const vector<string> &userIds,
                                    const vector<double> &values= {}) override { // there is no need of values for equal split
        vector<Split> splits; // Resultant splits
        double amountPerUser = totalAmount / userIds.size();

        // Validation: that totalAmount can be equally divided among users
        if(abs(amountPerUser * userIds.size() ) != abs(totalAmount)){
            throw invalid_argument("Total amount cannot be equally split among users");
        }

        for(const string &userId : userIds){
            splits.push_back(Split(userId, amountPerUser)); // put each user's split into splits
        }
        return splits;
    }
};  
// Exact Split Strategy - Unequal split based on exact amounts provided
class ExactSplitStrategy : public SplitStrategy{
public:                         // values will contain exact amounts for each user
    vector<Split> calculateSplit(double totalAmount, const vector<string> &userIds,
                                    const vector<double> &values= {}) override { //  vector<double> &values= {} --> values will contain exact amounts for each user
        vector<Split> splits; // Resultant splits   

        // Validation: that the sum of exact amounts matches the total amount 
        // and number of users and values size should be same
        double sum = 0;
        for(double val : values){
            sum += val;
        }
        if(abs(sum - totalAmount) != 0 || userIds.size() != values.size()){
            throw invalid_argument("Exact amounts do not sum up to total amount or size mismatch");
        }

        for(int i=0; i<userIds.size(); i++){
            splits.push_back(Split(userIds[i], values[i])); // put each user's split into splits
        }
        return splits;
    }
};

// Percentage Split Strategy - Unequal split based on percentage provided
class PercentageSplitStrategy : public SplitStrategy{
public:                         // values will contain percentage for each user
    vector<Split> calculateSplit(double totalAmount, const vector<string> &userIds,
                                    const vector<double> &values= {}) override { //  vector<double> &values= {} --> values will contain percentage for each user
        vector<Split> splits; // Resultant splits   

        // Validation: that the sum of percentages is 100 
        // and number of users and values size should be same
        double sum = 0;
        for(double val : values){
            sum += val;
        }
        if(abs(sum - 100.0) != 0 || userIds.size() != values.size()){
            throw invalid_argument("Percentages do not sum up to 100 or size mismatch");
        }

        for(int i=0; i<userIds.size(); i++){
            double amount = (values[i] * totalAmount) / 100.0; // Formula = value[i] = percentage for user i * totalAmount / 100
            splits.push_back(Split(userIds[i], amount)); // put each user's split into splits
        }
        return splits;
    }
};


// Factory for split strategies
class SplitStrategyFactory {
public:
    static SplitStrategy* getSplitStrategy(SplitType type) {
        switch (type) {
            case SplitType::EQUAL:
                return new EqualSplitStrategy();
            case SplitType::EXACT:
                return new ExactSplitStrategy();
            case SplitType::PERCENTAGE:
                return new PercentageSplitStrategy();
            default:
                throw invalid_argument("Invalid Split Type");
        }
    }
};