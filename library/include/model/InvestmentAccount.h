#pragma once
#include "Account.h"
#include "GoldCoins.h"

/**
 * Exception thrown on errors with buying/selling gold
 */
class GoldError : public std::logic_error {
public:
    explicit GoldError(const std::string& msg);
};

/**
 * Investment Account
 */
class InvestmentAccount : public Account {
public:
    /**
     * Parameterized constructor
     * @param accountNumber Account number
     * @param id Client ID of the account holder
     */
    InvestmentAccount(const std::string &accountNumber, const std::string &id);

    /**
     * Function allowing the purchase of gold coins
     * @param count Number of gold coins to purchase
     */
    void buyGold(int count);

    /**
     * Function allowing the sale of gold coins
     * @returns Earned amount
     */
    Amount sellGold();

    /**
     * Function returning object properties as a string
     * @return Returns information about the account
     */
    std::string toString() const override;

    /**
     * @returns All information about the object in JSON format, needed to restore its state when reading from a file
     */
    json toJSON() const override;

    /**
     * @returns Account type
     */
    AccountType getType() const override;

    /**
     * @returns Gold coins object if assigned to the account
     */
    std::shared_ptr<GoldCoins> getGoldCoins() const;

    /**
     * Function returning the transfer fee
     * @return Transfer fee
     */
    Amount getFee() override;

    /**
     * Creates an object (taking into account the appropriate account type) based on information
     * in JSON format
     * @param source Information about the object in JSON format
     * @returns Created object
     */
    static std::shared_ptr<Account> fromJSON(const json& source);
private:
    // gold coins
    std::shared_ptr<GoldCoins> goldCoins;

    /// Transfer fee
    Amount transferFee = 5;

    /**
     * Constructor from JSON
     * @param source Information about the account in JSON format
     */
    explicit InvestmentAccount(const json& source);
};
