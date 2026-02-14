#pragma once
#include <boost/date_time.hpp>

#include "Amount.h"
#include "Serializable.h"

namespace pt = boost::posix_time;

/**
 * Class representing a certain amount of gold coins
 */
class GoldCoins : public Serializable {
public:
    /**
     * Constructor
     * @param count Number of gold coins
     * @param purchaseTime Purchase time
     */
    explicit GoldCoins(
        int count,
        pt::ptime purchaseTime = pt::second_clock::local_time()
    );

    /**
     * @returns Number of gold coins
     */
    int getCount() const;

    /**
     * @return Gold coins purchase time
     */
    pt::ptime getPurchaseTime() const;

    /**
     * Calculates the current value of gold coins.
     * The price of a gold coin increases by one zloty every day.
     * @param date Time for which to calculate the value
     */
    Amount calculateValue(pt::ptime date = pt::second_clock::local_time());

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
     * Creates an object from data in JSON format
     * @param source Information about the object in JSON format
     * @returns Gold coins object
     */
    static std::shared_ptr<GoldCoins> fromJSON(const json& source);
private:
    int count;
    boost::posix_time::ptime purchaseTime;
};
