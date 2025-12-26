#include<bits/stdc++.h>
using namespace std;

class DebtSimplifier {
public:
    static map<string, map<string, double>> simplifyDebts(
        map<string, map<string, double>> groupBalances) {
        
        // Calculate net amount for each person
        map<string, double> netAmounts;
        
        // Initialize all users with 0
        for (const auto& userBalance : groupBalances) {
            netAmounts[userBalance.first] = 0;
        }
        
        // Calculate net amounts
        // We only need to process each balance once (not twice)
        // If groupBalances[A][B] = 200, it means B owes A 200
        // So A should receive 200 (positive) and B should pay 200 (negative)
        for (const auto& userBalance : groupBalances) {
            string creditorId = userBalance.first;
            for (const auto& balance : userBalance.second) {
                string debtorId = balance.first;
                double amount = balance.second;
                
                // Only process positive amounts to avoid double counting
                if (amount > 0) {
                    netAmounts[creditorId] += amount;  // creditor receives
                    netAmounts[debtorId] -= amount;    // debtor pays
                }
            }
        }
        
        // Divide users into creditors and debtors
        vector<pair<string, double>> creditors; // those who should receive money
        vector<pair<string, double>> debtors;   // those who should pay money
        
        for (const auto& net : netAmounts) {
            if (net.second > 0.01) { // creditor
                creditors.push_back({net.first, net.second});
            } else if (net.second < -0.01) { // debtor
                debtors.push_back({net.first, -net.second}); // store positive amount
            }
        }
        
        // Sort for better optimization (largest amounts first)
        sort(creditors.begin(), creditors.end(), 
             [](const pair<string, double>& a, const pair<string, double>& b) {
                 return a.second > b.second;
             });
        sort(debtors.begin(), debtors.end(), 
             [](const pair<string, double>& a, const pair<string, double>& b) {
                 return a.second > b.second;
             });
        
        // Create new simplified balance map
        map<string, map<string, double>> simplifiedBalances;
        
        // Initialize empty maps for all users
        for (const auto& userBalance : groupBalances) {
            simplifiedBalances[userBalance.first] = map<string, double>();
        }
        
        // Use greedy algorithm to minimize transactions
        int i = 0, j = 0;
        while (i < creditors.size() && j < debtors.size()) {
            string creditorId = creditors[i].first;
            string debtorId = debtors[j].first;
            double creditorAmount = creditors[i].second;
            double debtorAmount = debtors[j].second;
            
            // Find the minimum amount to settle
            double settleAmount = min(creditorAmount, debtorAmount);
            
            // Update simplified balances
            // debtorId owes creditorId the settleAmount
            simplifiedBalances[creditorId][debtorId] = settleAmount;
            simplifiedBalances[debtorId][creditorId] = -settleAmount;
            
            // Update remaining amounts
            creditors[i].second -= settleAmount;
            debtors[j].second -= settleAmount;
            
            // Move to next creditor or debtor if current one is settled
            if (creditors[i].second < 0.01) {
                i++;
            }
            if (debtors[j].second < 0.01) {
                j++;
            }
        }
        
        return simplifiedBalances;
    }
};
