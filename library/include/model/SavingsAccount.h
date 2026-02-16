#pragma once
#include "Account.h"
#include "Deposit.h"
#include <boost/date_time.hpp>

namespace gr = boost::gregorian;

/**
 * Savings Account
 */
class SavingsAccount : public Account {
public:
    /**
     * Parameterized constructor
     * @param accountNumber Account number
     * @param id Client ID of the account holder
     */
    SavingsAccount(const std::string &accountNumber, const std::string &id);

    /**
     * Function creating a deposit
     * @param amount Deposit amount
     * @param beginTime Deposit start date
     */
    bool createDeposit(Amount amount, const gr::date &beginTime = gr::day_clock::local_day());

    /**
     * @returns Pointer to the deposit object (if one is established)
     */
    std::shared_ptr<Deposit> getDeposit() const;

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
     * Ends the deposit
     */
    void deleteDeposit();

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
    /// Transfer fee
    Amount transferFee = 10;

    /// Pointer to the deposit object
    std::shared_ptr<Deposit> deposit = nullptr;

    /**
     * Constructor from JSON
     * @param source Information about the account in JSON format
     */
    explicit SavingsAccount(const json& source);
};
