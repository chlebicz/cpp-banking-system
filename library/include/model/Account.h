#pragma once
#include <string>
#include "Amount.h"
#include "Card.h"
#include <memory>
#include <vector>

#include "Entity.h"
#include "Serializable.h"

/**
 * Account type
 */
enum class AccountType {
    Main,
    Savings,
    Investment,
    Crypto
};

/**
 * Exception thrown when there are not enough funds on the account to perform
 * a given operation
 */
class NotEnoughMoney : public std::logic_error {
public:
    /**
     * Constructor
     * @param msg Message allowing to evaluate the source of the error
     */
    explicit NotEnoughMoney(const std::string& msg)
        : std::logic_error(msg) {}
};

/**
 * Account
 */
class Account : public Entity {
public:
    /**
     * Parameterized constructor
     * @param accountNumber Account number
     * @param ownerID Client ID of the account holder
     */
    Account(std::string accountNumber, std::string ownerID);

    /**
     * Constructor from file data
     * @param accountNumber Account number
     * @param ownerID Client ID of the account holder
     * @param balance Account balance
     * @param cards Vector of cards assigned to the account
     */
    Account(
        std::string accountNumber, std::string ownerID, Amount balance,
        std::vector<std::shared_ptr<Card>> cards
    );

    /**
     * Balance getter
     * @returns Returns the account balance
     */
    Amount getBalance() const;

    /**
     * Balance setter
     * @param value Amount to set as the balance
     */
    void setBalance(const Amount &value);

    /**
     * Account number getter
     * @returns Returns the account number
     */
    const std::string& getAccountNumber() const;

    /**
     * Account holder PESEL getter
     * @returns Returns PESEL
     */
    const std::string& getOwnerID() const;

    /**
    * Getter for cards assigned to the account
    * @returns Returns a vector of cards
    */
    const std::vector<std::shared_ptr<Card>>& getCards() const;

    /**
     * Adding cards to the account
     * @param card Card to assign to the account
     */
    void addCard(const std::shared_ptr<Card> &card);

    /**
     * Removing cards from the account
     * @param card Card to remove from the account
     */
    void removeCard(const std::shared_ptr<Card> &card);

    /**
     * Function performing transactions
     * @param amount Transaction amount
     * @param account Account to which the transaction is made
     */
    bool transaction(const Amount &amount, const std::shared_ptr<Account> &account);

    /**
     * @return Significant information about the object saved in a user-friendly format
     */
    std::string toString() const override;

    /**
     * @returns Unique object identifier
     */
    std::string getId() const override;

    /**
     * @returns All information about the object in JSON format, needed to restore its state when reading from a file
     */
    json toJSON() const override;

    /**
     * @returns Account type
     */
    virtual AccountType getType() const = 0;

    /**
     * Creates an object (taking into account the appropriate account type) based on information
     * in JSON format
     * @param source Information about the object in JSON format
     * @returns Created object
     */
    static std::shared_ptr<Account> fromJSON(const json& source);

    /**
     * Function returning the fee for transfers using the account
     * @return Returns the transfer fee
     */
    virtual Amount getFee();
private:
    std::string accountNumber, ownerID;
    Amount transferFee = 0;
    Amount balance = 0;
    std::vector<std::shared_ptr<Card>> cards;
protected:
    /**
     * Constructor from JSON
     * @param source Information about the account in JSON format
     */
    explicit Account(const json& source);
};
