# Expense Sharing Application Design — C++

A console-based expense-sharing app inspired by Splitwise. It supports **group expenses**, **individual (1–1) expenses**, **multiple split strategies**, **settlements**, **notifications**, and **debt simplification**.


---
## Plan - Initial Design 

This was the initial plan before implementation:

![Initial Plan](assests/plan.jpg)

## Features

### 1) Groups
- Create a group and add members.
- Add expenses inside a group.
- View group-specific balances.

### 2) Split Strategies
Supports multiple ways to split an expense:
- **EQUAL**: split amount equally among involved users
- **EXACT**: split by exact amounts per user
- **PERCENTAGE**: split by percentages per user

### 3) Settlements
- Members can settle dues inside a group (payment from one user to another).
- Prevents leaving a group if a user has pending balances.

### 4) Individual Expenses (Outside groups)
- Add an individual expense between two users (1–1).

### 5) Notifications (Observer Pattern)
- When an expense is added or a settlement happens in a group, all group members get a notification (console message).

### 6) Debt Simplification
- Simplifies group balances to reduce the number of transactions using a greedy settlement approach.

---

## Design / Architecture Notes

This project uses common OOP patterns:

- **Observer Pattern**
  - `Observer` interface with `update(message)`
  - `User` implements `Observer`
  - `Group::notifyMembers()` broadcasts updates

- **Strategy Pattern**
  - `SplitStrategy` interface
  - `EqualSplitStrategy`, `ExactSplitStrategy`, `PercentageSplitStrategy`

- **Factory Pattern**
  - `SplitStrategyFactory::getSplitStrategy(SplitType)`

- **Singleton / Facade**
  - `Splitwise` is a singleton that provides a simpler API to create users/groups and add expenses/settlements.

---

## Project Plan / Requirements (from the initial design notes)

1. User can **join / leave a group**  
2. User can **add expense in a group**  
3. User can **settle** an expense in a group (settlement = transaction to clear balances)  
4. Adding expense should be based on **external splitting strategy**  
   - Equal split, Exact split, Percentage split  
   - Designed to be extensible for future strategies  
5. User **can’t leave a group without settling** outstanding balances  
6. User can add **individual expense** (one-on-one, outside group)  
7. **Notifications** are sent when an expense is **added** or **settled**  
8. Group expense book can be **simplified** using a **debt simplification algorithm** to reduce transactions (greedy approach)

---

## How to Build & Run (macOS)

From the project folder:

```bash
cd "/Users/harshvardhan/Documents/Placements/Projects/SplitWise Clone"
g++ -std=c++17 -O2 -Wall -Wextra -pedantic ExpenseSharing.cpp -o ExpenseSharing
./ExpenseSharing
```

> If `g++` isn’t available: `xcode-select --install`

---

## Demo Scenario (What `main()` does)
- Creates users
- Creates a group and adds members
- Adds group expenses (equal + exact)
- Shows balances
- Simplifies debts and shows balances again
- Adds an individual expense
- Shows per-user balances
- Attempts to remove a user (blocked if balances exist)
- Performs a settlement and retries removal

---

## Limitations
- Notifications are currently console-based.
- Data is in-memory (no database / persistence).
- Floating point comparisons use small thresholds (e.g., `0.01`) for cleanup.

---

## Future Improvements
- Persist users/groups/expenses (file/DB).
- Better rounding handling for equal splits (paise-level distribution).
- CLI menu / interactive input (instead of a fixed `main()` demo).
- Unit tests for split strategies and debt simplification.
