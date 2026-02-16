#pragma once
#include <memory>
#include "Amount.h"
#include "Serializable.h"

/**
 * Card type
 */
enum class CardType {
    Standard = 0,
    Gold = 1,
    Diamond = 2
};

/**
 * Card
 */
class Card : public Serializable {
public:
    /**
     * Method calculating the fee for a card transaction
     * @param amount Amount from which the commission is to be calculated
     */
    virtual Amount calculateFee(const Amount& amount) const = 0;

    /**
     * @returns All information about the object in JSON format, needed to restore its state when reading from a file
     */
    json toJSON() const override;

    /**
     * Creates an object based on information in JSON format
     * @param source Information about the object in JSON format
     * @returns Created object
     */
    static std::shared_ptr<Card> fromJSON(const json& source);

    /**
     * @returns Type of the given card
     */
    virtual CardType getType() const = 0;

    /**
     * Method returning the price of the card
     * @return Card price
     */
    virtual Amount getPrice() const = 0;
protected:
    /// Basic transaction fee
    Amount baseFee = 2;
};
