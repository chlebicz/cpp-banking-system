#pragma once
#include "Card.h"

/**
 * Gold Card
 */
class GoldCard : public Card {
public:
    /**
     * Parameterless constructor
     */
    GoldCard();

    /**
     * Method calculating the fee for a card transaction
     * @param amount Amount from which the commission is to be calculated
     */
    Amount calculateFee(const Amount &amount) const override;

    /**
     * @returns Type of the given card
     */
    CardType getType() const override;

    /**
     * @return Significant information about the object saved in a user-friendly format
     */
    std::string toString() const override;

    /**
     * Method returning the price of the card
     * @return Card price
     */
    Amount getPrice() const;
private:
    Amount price = 100;
};
