#pragma once
#include "Amount.h"
#include <boost/date_time.hpp>

namespace gr = boost::gregorian;

class Deposit : public Serializable {
public:
    /**
     * Parameterized constructor
     * @param amount Deposit amount
     * @param beginTime Deposit start date
     */
    Deposit(const Amount &amount, const gr::date &beginTime);

    /**
     * Deposit amount getter
     * @returns Returns the deposit amount
     */
    Amount getAmount() const;

    /**
     * Deposit start date getter
     * @returns Returns the deposit start date
     */
    gr::date getBeginTime() const;

    /**
     * Function ending the deposit, calculates the amount that should be returned to the account
     * @returns Returns the deposit amount plus interest
     */
    Amount endDeposit();

    /**
     * Function returning object properties as a string
     * @returns Returns deposit data
     */
    std::string toString() const override;

    /**
     * @returns All information about the object in JSON format, needed to restore its state when reading from a file
     */
    json toJSON() const override;

    /**
     * Creates an object based on information in JSON format
     * @param source Information about the object in JSON format
     * @returns Created object
     */
    static std::shared_ptr<Deposit> fromJSON(const json& j);
private:
    /// Deposit amount
    Amount amount;

    /// Deposit start date
    gr::date beginTime;
};
