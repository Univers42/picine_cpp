/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Account.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 01:20:32 by marvin            #+#    #+#             */
/*   Updated: 2026/01/29 16:49:40 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Account.hpp"

#include <ctime>
#include <iomanip>
#include <iostream>

// diff <(sed 's/^\[[^]]*\] //' out.log) <(sed 's/^\[[^]]*\] //' 19920104_091532.log)

/**
 * @defgroup StaticMembers Static member definitions
 * @brief Class-wide counters shared across all Account instances.
 *
 * These are defined here (not in the header) because static data members
 * need exactly one definition in a translation unit. They track the global
 * state of the banking system: how many accounts exist, the total funds held,
 * and the cumulative number of deposit / withdrawal operations performed.
 */
int Account::_nbAccounts = 0;
int Account::_totalAmount = 0;
int Account::_totalNbDeposits = 0;
int Account::_totalNbWithdrawals = 0;

/**
 * @brief Private default constructor — intentionally left empty.
 *
 * Declared private so that no code outside the class can default-construct
 * an Account without an initial deposit. This enforces the invariant that
 * every valid Account is created with an explicit starting balance.
 */
Account::Account(void) {}

/**
 * @brief Parameterised constructor — creates a new bank account.
 *
 * Assigns the next available index to this account (using the current value
 * of the class-wide counter before incrementing it), stores the initial
 * deposit, resets per-account operation counters to zero, then updates the
 * class-wide totals. Prints a timestamped creation log line to stdout.
 *
 * Output format:
 * @code
 * [YYYYMMDD_HHMMSS] index:<n>;amount:<initial_deposit>;created
 * @endcode
 *
 * @param initial_deposit  The opening balance for this account (in cents /
 *                         arbitrary currency units). Must be >= 0 in practice.
 */
Account::Account(int initial_deposit)
    : _accountIndex(_nbAccounts),
      _amount(initial_deposit),
      _nbDeposits(0),
      _nbWithdrawals(0) {
  _nbAccounts++;
  _totalAmount += initial_deposit;
  _displayTimestamp();
  std::cout << " index:" << _accountIndex << ";amount:" << _amount << ";created"
            << std::endl;
}

/**
 * @brief Destructor — closes the account and logs the event.
 *
 * Called automatically when an Account object goes out of scope or is
 * explicitly destroyed (e.g. when the vector holding all accounts is
 * destroyed at the end of main). Prints a timestamped closing line so the
 * output log records the final balance at the moment of destruction.
 *
 * Output format:
 * @code
 * [YYYYMMDD_HHMMSS] index:<n>;amount:<current_balance>;closed
 * @endcode
 *
 * @note Does NOT decrement _nbAccounts or _totalAmount — the class counters
 *       reflect lifetime totals, not "currently open" totals.
 */
Account::~Account(void) {
  _displayTimestamp();
  std::cout << " index:" << _accountIndex << ";amount:" << _amount << ";closed"
            << std::endl;
}

/**
 * @brief Returns the total number of Account objects ever constructed.
 *
 * Because _nbAccounts is a static member it is shared by all instances.
 * The value is incremented in the parameterised constructor and never
 * decremented, so it represents a lifetime count, not the number of
 * currently open accounts.
 *
 * @return int  Total account count.
 */
int Account::getNbAccounts(void) { return _nbAccounts; }

/**
 * @brief Returns the current combined balance across all accounts.
 *
 * _totalAmount is updated on every deposit and withdrawal, so it always
 * reflects the live sum of every account's _amount field.
 *
 * @return int  Sum of all account balances.
 */
int Account::getTotalAmount(void) { return _totalAmount; }

/**
 * @brief Returns the total number of deposit operations made across all
 *        accounts since the program started.
 *
 * @return int  Cumulative deposit count.
 */
int Account::getNbDeposits(void) { return _totalNbDeposits; }

/**
 * @brief Returns the total number of successful withdrawal operations made
 *        across all accounts since the program started.
 *
 * Refused withdrawals are NOT counted here.
 *
 * @return int  Cumulative successful withdrawal count.
 */
int Account::getNbWithdrawals(void) { return _totalNbWithdrawals; }

/**
 * @brief Prints a timestamped summary of global banking statistics.
 *
 * Outputs a single line containing the total number of accounts, the
 * combined balance, the cumulative deposit count, and the cumulative
 * withdrawal count. Useful as a snapshot before and after bulk operations.
 *
 * Output format:
 * @code
 * [YYYYMMDD_HHMMSS] accounts:<n>;total:<sum>;deposits:<d>;withdrawals:<w>
 * @endcode
 */
void Account::displayAccountsInfos(void) {
  _displayTimestamp();
  std::cout << " accounts:" << getNbAccounts() << ";total:" << getTotalAmount()
            << ";deposits:" << getNbDeposits()
            << ";withdrawals:" << getNbWithdrawals() << std::endl;
}

/**
 * @brief Credits an amount to this account and updates global totals.
 *
 * Logs the operation with the balance before the deposit (p_amount), the
 * deposit value, and the resulting balance. Both the per-account deposit
 * counter (_nbDeposits) and the class-wide counter (_totalNbDeposits) are
 * incremented. The class-wide total balance (_totalAmount) is also updated.
 *
 * Output format:
 * @code
 * [YYYYMMDD_HHMMSS] index:<n>;p_amount:<before>;deposit:<d>;amount:<after>;nb_deposits:<count>
 * @endcode
 *
 * @param deposit  The amount to add to this account. Assumed to be positive.
 */
void Account::makeDeposit(int deposit) {
  _displayTimestamp();
  std::cout << " index:" << _accountIndex << ";p_amount:" << _amount
            << ";deposit:" << deposit;
  _amount += deposit;
  _totalAmount += deposit;
  _nbDeposits++;
  _totalNbDeposits++;
  std::cout << ";amount:" << _amount << ";nb_deposits:" << _nbDeposits
            << std::endl;
}

/**
 * @brief Attempts to debit an amount from this account.
 *
 * First checks whether the account holds sufficient funds. If the requested
 * withdrawal exceeds the current balance the operation is refused: a
 * "refused" message is printed and the function returns false without
 * modifying any balance or counter.
 *
 * On success, the balance is reduced, _totalAmount is updated, and both the
 * per-account (_nbWithdrawals) and class-wide (_totalNbWithdrawals) counters
 * are incremented.
 *
 * Output format (refused):
 * @code
 * [YYYYMMDD_HHMMSS] index:<n>;p_amount:<before>;withdrawal:refused
 * @endcode
 *
 * Output format (success):
 * @code
 * [YYYYMMDD_HHMMSS] index:<n>;p_amount:<before>;withdrawal:<w>;amount:<after>;nb_withdrawals:<count>
 * @endcode
 *
 * @param withdrawal  The amount to subtract from this account.
 * @return true   Withdrawal succeeded and balances were updated.
 * @return false  Insufficient funds — no state was changed.
 */
bool Account::makeWithdrawal(int withdrawal) {
  _displayTimestamp();
  std::cout << " index:" << _accountIndex << ";p_amount:" << _amount
            << ";withdrawal:";
  if (withdrawal > _amount) {
    std::cout << "refused" << std::endl;
    return (false);
  }
  _amount -= withdrawal;
  _totalAmount -= withdrawal;
  _nbWithdrawals++;
  _totalNbWithdrawals++;
  std::cout << withdrawal << ";amount:" << _amount
            << ";nb_withdrawals:" << _nbWithdrawals << std::endl;
  return (true);
}

/**
 * @brief Returns the current balance of this account without logging.
 *
 * A simple, non-mutating accessor. Marked const because it does not alter
 * any member state.
 *
 * @return int  The account's current balance.
 */
int Account::checkAmount(void) const { return (_amount); }

/**
 * @brief Prints a timestamped status line for this individual account.
 *
 * Shows the account index, current balance, total deposits made into this
 * account, and total withdrawals taken from it. Marked const because it
 * only reads member data.
 *
 * Output format:
 * @code
 * [YYYYMMDD_HHMMSS] index:<n>;amount:<bal>;deposits:<d>;withdrawals:<w>
 * @endcode
 */
void Account::displayStatus(void) const {
  _displayTimestamp();
  std::cout << " index:" << _accountIndex << ";amount:" << _amount
            << ";deposits:" << _nbDeposits << ";withdrawals:" << _nbWithdrawals
            << std::endl;
}

/**
 * @brief Writes a formatted UTC-local timestamp to stdout (no newline).
 *
 * Retrieves the current wall-clock time via std::time(), converts it to a
 * local broken-down time struct with std::localtime(), then prints it in
 * the format [YYYYMMDD_HHMMSS] using setfill/setw for zero-padding. The
 * opening bracket is printed but no trailing space or newline — callers
 * are expected to append the rest of their log line immediately after.
 *
 * Declared static because it does not depend on any particular instance's
 * data; it is a shared utility used by every logging method in the class.
 *
 * Output example:
 * @code
 * [20260302_173058]
 * @endcode
 */
void Account::_displayTimestamp(void) {
  std::time_t t = std::time(NULL);
  std::tm* tm = std::localtime(&t);

  std::cout << "[" << std::setfill('0') << std::setw(4) << (tm->tm_year + 1900)
            << std::setw(2) << (tm->tm_mon + 1) << std::setw(2) << tm->tm_mday
            << "_" << std::setw(2) << tm->tm_hour << std::setw(2) << tm->tm_min
            << std::setw(2) << tm->tm_sec << "]";
}
