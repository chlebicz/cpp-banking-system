#pragma once
#include "Account.h"

class CryptoAccount : public Account {
public:
    /**
     * Parameterized constructor
     * @param accountNumber Account number
     * @param id Client ID of the account holder
     */
    CryptoAccount(const std::string &accountNumber, const std::string &id);

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
     * Function returning the transfer fee
     * @return Transfer fee
     */
    Amount getFee() override;

    /**
     * Creates an object based on information in JSON format
     * @param source Information about the object in JSON format
     * @returns Created object
     */
    static std::shared_ptr<Account> fromJSON(const json& source);

    /// Destructor
    ~CryptoAccount() override = default;
private:
    /// Transaction fee
    Amount transferFee = 100;

    /**
     * Constructor from JSON
     * @param source Information about the account in JSON format
     */
    explicit CryptoAccount(const json& source);
};
