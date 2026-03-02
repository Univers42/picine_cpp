# ex02 — The Account Class

## Subject

Reconstruct a pre-existing `Account` class from its header file (`Account.hpp`) and a reference log (`19920104_091532.log`). The provided `tests.cpp` runs a sequence of deposits and withdrawals using STL iterators, and your output — once the timestamp is stripped — must match the reference log byte for byte.

```
$> ./ex02 2>&1 | sed 's/^\[[^]]*\] //' | diff - 19920104_091532.log
```

No diff output means the exercise is complete.

---

## What Is the Challenge?

You are given:
- The **header** (`Account.hpp`) — public API, types, static members
- The **test harness** (`tests.cpp`) — a fixed sequence of operations
- The **expected output** (`19920104_091532.log`) — the ground truth

You must write `Account.cpp` to make the output match exactly. The challenge is to understand what each line of output encodes, reverse-engineer the exact format, and correctly implement the accounting logic for deposits, withdrawals, and global statistics.

---

## Concepts Learned

| Concept | Where used |
|---|---|
| Static class members (shared state) | `_nbAccounts`, `_totalAmount`, etc. |
| Static member initialization | `int Account::_nbAccounts = 0` in `.cpp` |
| Constructor / destructor lifecycle | Creation and destruction log lines |
| `std::time` and `std::localtime` | `_displayTimestamp()` |
| `std::setw` / `std::setfill` | Zero-padded timestamp formatting |
| STL iterators | Pair-iterator loop in `tests.cpp` |
| `std::vector` from a C array | `accounts_t accounts(amounts, amounts + size)` |
| `std::pair` as a zip iterator | `acc_int_t` simultaneous iteration |
| `typedef` for type aliases | All type aliases in `tests.cpp` |

---

## How the Static Members Work

### Declaration (in the header)

```cpp
// Account.hpp
class Account {
  static int _nbAccounts;       // shared: one copy for all Account objects
  static int _totalAmount;
  static int _totalNbDeposits;
  static int _totalNbWithdrawals;
};
```

`static` member variables are **class-level** variables. They don't live inside any instance — there is literally one integer in memory shared by every `Account` object ever created.

### Definition (mandatory in the `.cpp`)

```cpp
// Account.cpp — MUST be here, exactly once
int Account::_nbAccounts       = 0;
int Account::_totalAmount      = 0;
int Account::_totalNbDeposits  = 0;
int Account::_totalNbWithdrawals = 0;
```

This is the **definition** (allocates storage). The `static` keyword in the header is only a **declaration**. Omitting this `.cpp` definition gives a linker error: `undefined reference to 'Account::_nbAccounts'`.

### Why static members for global counters?

Because every `Account` constructor, destructor, deposit, and withdrawal must update a running total that is accessible without having a specific account in hand (`Account::getNbAccounts()` is also `static` — callable with no instance). The alternative — a global variable — would break encapsulation. The static member is global state that belongs to the class.

---

## The Constructor and Destructor — The Logging Protocol

### Constructor

```cpp
Account::Account(int initial_deposit)
    : _accountIndex(_nbAccounts),  // index = current count BEFORE increment
      _amount(initial_deposit),
      _nbDeposits(0),
      _nbWithdrawals(0)
{
  _nbAccounts++;              // global: one more account exists
  _totalAmount += initial_deposit;
  _displayTimestamp();
  std::cout << " index:" << _accountIndex
            << ";amount:" << _amount
            << ";created" << std::endl;
}
```

Each account **captures its index at birth**: `_accountIndex = _nbAccounts` before `_nbAccounts++`. So the first account is index 0, the second is index 1, regardless of what happens later.

The constructor prints:
```
[19920104_091532] index:0;amount:42;created
```

### Destructor

```cpp
Account::~Account(void) {
  _displayTimestamp();
  std::cout << " index:" << _accountIndex
            << ";amount:" << _amount
            << ";closed" << std::endl;
}
```

Prints:
```
[19920104_091532] index:0;amount:47;closed
```

Note the static counters (`_nbAccounts`, `_totalAmount`) are **not decremented** in the destructor — the reference log doesn't update global stats on close.

---

## How the Fake Data Was Retrieved — Reverse-Engineering the Log

The reference log `19920104_091532.log` is the key artifact. The filename encodes the timestamp: **January 4, 1992 at 09:15:32**. Here is how you decode every line:

### Step 1: Strip the timestamp

```bash
sed 's/^\[[^]]*\] //' 19920104_091532.log
```

This removes `[YYYYMMDD_HHMMSS] ` from every line, leaving pure account data.

### Step 2: Read the creation block

```
accounts:8;total:20049;deposits:0;withdrawals:0
index:0;amount:42;deposits:0;withdrawals:0
index:1;amount:54;deposits:0;withdrawals:0
...
```

This tells you the 8 initial amounts: **42, 54, 957, 432, 1234, 0, 754, 16576**. These are the values in `tests.cpp`:

```cpp
int const amounts[] = {42, 54, 957, 432, 1234, 0, 754, 16576};
```

### Step 3: Read the deposit block

```
index:0;p_amount:42;deposit:5;amount:47;nb_deposits:1
index:1;p_amount:54;deposit:765;amount:819;nb_deposits:1
...
```

`p_amount` = previous amount, `deposit` = deposit value, `amount` = new amount. This tells you the deposit array: **5, 765, 564, 2, 87, 23, 9, 20** — exactly `int const d[]` in `tests.cpp`.

### Step 4: Read the withdrawal block

```
index:0;p_amount:47;withdrawal:321;amount:-274  → refused (negative result)
index:1;p_amount:819;withdrawal:34;amount:785;nb_withdrawals:1
...
```

A refused withdrawal prints `withdrawal:refused` instead of the amount. This reveals which withdrawals exceeded the account balance, and tells you the withdrawal array: **321, 34, 657, 4, 76, 275, 657, 7654** — `int const w[]` in `tests.cpp`.

The log is a **complete audit trail** — every operation is logged with before-and-after values, making it possible to reconstruct every input array purely by reading the output. This is the exercise's subtle trick: the "fake data" in `tests.cpp` was never hidden — it was always visible in the log, if you knew how to read it.

---

## How `tests.cpp` Drives Everything

### Creating accounts from a C array

```cpp
int const amounts[] = {42, 54, 957, 432, 1234, 0, 754, 16576};
size_t const amounts_size(sizeof(amounts) / sizeof(int)); // = 8

typedef std::vector<Account::t> accounts_t;
accounts_t accounts(amounts, amounts + amounts_size);
//                  ^       ^
//                  begin   end (one-past-last element)
```

`std::vector`'s iterator-range constructor copies elements from `[begin, end)`. Since `Account::t` is `typedef Account t`, each element of `amounts` is **implicitly converted** to an `Account` by the single-argument constructor `Account(int initial_deposit)`. This is why the constructor is marked `explicit` in the header — to prevent this kind of conversion happening *accidentally* elsewhere.

Wait — if it's `explicit`, how does the vector construction work? The vector uses **direct initialization** internally (equivalent to `Account a(amounts[i])`), which is allowed even with `explicit`. Implicit conversion (e.g., `Account a = 42;`) would be rejected.

### The pair-iterator zip loop

```cpp
typedef std::pair<accounts_t::iterator, ints_t::iterator> acc_int_t;

for (acc_int_t it(acc_begin, dep_begin);
     it.first != acc_end && it.second != dep_end;
     ++(it.first), ++(it.second))
{
  (*(it.first)).makeDeposit(*(it.second));
}
```

`std::pair` holds two iterators simultaneously. Incrementing both on each loop step steps through both vectors in lockstep — `accounts[0]` with `deposits[0]`, `accounts[1]` with `deposits[1]`, etc. This is a manual ZIP iteration, equivalent to Python's `zip(accounts, deposits)`. C++98 has no `std::zip`, so this pair trick is the idiomatic workaround.

### Global stats update flow

```
Account::displayAccountsInfos()
  → reads _nbAccounts, _totalAmount, _totalNbDeposits, _totalNbWithdrawals
  → all updated by constructors / makeDeposit / makeWithdrawal

Each account.displayStatus()
  → reads per-instance: _accountIndex, _amount, _nbDeposits, _nbWithdrawals
```

The three `displayAccountsInfos()` calls (before deposits, after deposits, after withdrawals) act as **checkpoints** that snapshot global state at three moments in time. Their output is what you compare against the log to verify correctness.

---

## The Timestamp — `_displayTimestamp()`

```cpp
void Account::_displayTimestamp(void) {
  std::time_t t = std::time(NULL);   // current calendar time
  std::tm* tm   = std::localtime(&t); // broken-down local time

  std::cout << "["
    << std::setfill('0') << std::setw(4) << (tm->tm_year + 1900)
    << std::setw(2)      << (tm->tm_mon + 1)   // tm_mon is 0-based!
    << std::setw(2)      << tm->tm_mday
    << "_"
    << std::setw(2)      << tm->tm_hour
    << std::setw(2)      << tm->tm_min
    << std::setw(2)      << tm->tm_sec
    << "]";
}
```

Key details:
- `tm_year` is **years since 1900** — you must add 1900
- `tm_mon` is **0-based** (January = 0) — you must add 1
- `std::setfill('0')` makes `setw(2)` pad with zeros: `9` → `09`
- `std::setfill` is **sticky** (persists until changed); `std::setw` resets after each use

This is why the diff command strips timestamps before comparing — you ran the binary today (2026), but the reference log was generated in 1992. The content after the timestamp must match; the timestamp itself is excluded.

---

## Withdrawal: Refused vs. Accepted

```cpp
bool Account::makeWithdrawal(int withdrawal) {
  _displayTimestamp();
  std::cout << " index:" << _accountIndex << ";p_amount:" << _amount
            << ";withdrawal:";
  if (withdrawal > _amount) {
    std::cout << "refused" << std::endl;
    return false;             // global counters NOT updated
  }
  _amount       -= withdrawal;
  _totalAmount  -= withdrawal;
  _nbWithdrawals++;
  _totalNbWithdrawals++;
  std::cout << withdrawal << ";amount:" << _amount
            << ";nb_withdrawals:" << _nbWithdrawals << std::endl;
  return true;
}
```

The format branches at `";withdrawal:"`:
- **Refused**: `";withdrawal:refused"` — nothing else, no `;amount:`, no counter update
- **Accepted**: `";withdrawal:321;amount:-279;nb_withdrawals:1"` — full line with new balance

The reference log shows account 0 (`amount:47`) refusing a withdrawal of 321 (47 < 321). This is visible as `withdrawal:refused` in the log, which is how the test arrays were confirmed.

---

## Key Takeaways

1. **Static members** = class-level shared state. Declaration in header, definition in `.cpp`. One copy in memory, accessible via class name or any instance.
2. **Constructor initializer list** assigns `_accountIndex = _nbAccounts` *before* the body increments `_nbAccounts`, guaranteeing unique sequential indices.
3. **Log as specification** — the reference log is a complete audit trail. Strip the timestamp, read the numbers, and you recover every input value. This is how `tests.cpp`'s arrays were derived.
4. **Pair-iterator zip** — `std::pair<iter, iter>` is C++98's way to iterate two containers simultaneously, incrementing both cursors each step.
5. **Vector from C array** — `vector<T>(ptr, ptr + n)` uses the iterator-range constructor; each element is direct-initialized from the array value.
6. **`std::setfill('0')` is sticky** — set it once before the first `setw` and it applies to all subsequent `setw` calls until changed.
7. **`explicit` constructor** prevents silent implicit conversions while still allowing direct initialization inside containers.
