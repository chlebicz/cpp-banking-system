#pragma once
#include "Amount.h"
#include "managers/AccountManager.h"
#include "managers/ClientManager.h"
#include "managers/TransferManager.h"

/**
 * "Bankruptcy" exception means that the bank did not have funds to handle
 * clients. After throwing the exception, the application cannot function.
 */
class Bankruptcy : public std::runtime_error {
public:
    explicit Bankruptcy(const std::string& msg) : std::runtime_error(msg) {}
};


/**
 * Exception related to operating on an invalid account
 */
class InvalidAccountError : public std::logic_error {
public:
    /**
     * Constructor
     * @param msg Message allowing to evaluate the source of the error
     */
    explicit InvalidAccountError(const std::string& msg);
};

/**
 * Singleton gathering the entire state of the bank
 */
class Bank {
public:
    /**
     * @returns Singleton instance
     */
    static Bank& getInstance();

    /**
     * @returns Current bank account balance
     */
    Amount getBalance() const;

    /**
     * Changes the bank account balance
     * @param value New account balance
     */
    void setBalance(Amount value);

    /**
     * Increases the bank account balance by a set amount
     * @param by Amount
     */
    void increaseBalance(Amount by);

    /**
     * Decreases the bank account balance by a set amount
     * @param by Amount
     * @throws Bankruptcy When the bank has no money
     */
    void decreaseBalance(Amount by);

    /**
     * @returns Client manager
     */
    ClientManager& getClientManager();

    /**
     * @returns Transfer manager
     */
    TransferManager& getTransferManager();

    /**
     * @returns Account manager
     */
    AccountManager& getAccountManager();

    /**
     * Loads the state of all managers from files
     */
    void loadAll();

    /**
     * Saves the state of all managers to files
     */
    void saveAll();

    /**
     * Function mediating in opening an account
     * @param clientID Client PESEL
     * @param accountType Account type
     */
    void openAccount(std::string clientID, AccountType accountType);

    /**
     * Function mediating in taking a loan
     * @param months Loan duration
     * @param amount Loan amount
     * @param number Operational account number
     * @param clientID Client PESEL
     */
    void takeLoan(int months, Amount amount, std::string number, std::string clientID);

    /**
     * Function returning a vector with client accounts
     * @param clientID Client PESEL
     * @return Vector with accounts
     */
    std::vector<std::shared_ptr<Account>> checkAccounts(std::string clientID);

    /**
     * Function mediating in ordering a card for a client
     * @param number Account number
     * @param cardType Card type
     */
    void orderNewCard(std::string number, CardType cardType);

    /**
     * Function creating a deposit
     * @param number Account number
     * @param amount Deposit amount
     * @return Boolean value whether the operation succeeded
     */
    bool createDeposit(std::string number, Amount amount);

    /**
     * Function ending a deposit
     * @param number Account number
     */
    void endDeposit(std::string number);

    /**
     * Function returning information about client loans
     * @param clientID Client PESEL
     * @return
     */
    std::string loanInfo(std::string clientID);

    /**
     * Function returning information about a deposit
     * @param number Account number
     * @return Information about the deposit
     */
    std::string depositInfo(std::string number);

    /**
     * Function allowing to perform transactions
     * @param number Sender account
     * @param number2 Recipient account
     * @param amount Transaction amount
     * @return Boolean value whether the transaction succeeded
     */
    bool transaction(std::string number, std::string number2, Amount amount);
private:
    Bank();
    ~Bank();

    Amount balance = 10000000;
    ClientManager clientManager;
    TransferManager transferManager;
    AccountManager accountManager;
};
