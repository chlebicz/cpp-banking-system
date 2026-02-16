#pragma once
#include "Account.h"
#include "Amount.h"
#include <boost/date_time.hpp>

namespace gr = boost::gregorian;

/**
 * Loan
 */
class Loan : public Serializable {
public:
    /**
     * Parameterized constructor
     * @param months Loan duration
     * @param amount Loan amount
     * @param beginTime Loan start time
     * @param operationalAccount Account on which transfers related to the loan are handled
     */
    Loan(
        int months, Amount amount, gr::date beginTime,
        std::shared_ptr<Account> operationalAccount
    );

    /**
     * Parameterized constructor
     * @param months Loan duration
     * @param amount Loan amount
     * @param beginTime Loan start time
     * @param operationalAccountNumber Account on which transfers related to the loan are handled
     */
    Loan(
        int months, Amount amount, gr::date beginTime,
        std::string operationalAccountNumber
    );

    /**
     * Parameterized constructor
     * @param months Loan duration
     * @param amount Loan amount
     * @param beginTime Loan start time
     * @param operationalAccountNumber Account on which transfers related to the loan are handled
     * @param monthlyPayment Loan installment
     */
    Loan(
        int months, Amount amount, gr::date beginTime,
        std::string operationalAccountNumber, Amount monthlyPayment
    );

    /**
     * Takes a loan
     */
    void create();

    /**
     * Loan length getter
     * @return Returns the number of months for which the loan was taken
     */
    int getMonths() const;

    /**
     * Loan amount getter
     * @return Returns the loan amount
     */
    Amount getAmount() const;

    /**
     * Monthly loan payment getter
     * @return Returns the monthly loan payment amount
     */
    Amount getMontlyPayment() const;

    /**
     * Remaining months getter
     * @return Returns the number of months remaining to pay off the loan
     */
    int getRemainingMonths() const;

    /**
     * Loan start date getter
     * @return Returns the loan start date
     */
    gr::date getBeginTime() const;

    /**
     * Operational account getter
     * @return Operational account
     */
    std::shared_ptr<Account> getOperationalAccount() const;

    /**
     * Charges the client's account with the loan installment
     * @returns Whether charging the account succeeded
     */
    bool takeMoney();

    /**
     * Function returning information about the loan as a string
     * @return Returns information about the loan
     */
    std::string toString() const override;

    /**
     * @returns All information about the object in JSON format, needed to restore its state when reading from a file
     */
    json toJSON() const override;

    /**
     * Creates an object from data in JSON format
     * @param source Information about the object in JSON format
     * @returns Gold coins object
     */
    static std::shared_ptr<Loan> fromJSON(const json& source);
private:
    int months;
    int paidMonths = 0;
    Amount amount;
    Amount monthlyPayment = 0;
    std::string operationalAccountNumber;
    std::shared_ptr<Account> operationalAccount;

    /// Loan start date
    gr::date beginTime;
};
