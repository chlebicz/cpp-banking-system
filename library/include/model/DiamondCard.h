#pragma once
#include "Card.h"

/**
 * Diamond Card
 */
class DiamondCard : public Card {
public:
    /**
     * Parameterless constructor
     */
    DiamondCard();

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
    Amount getPrice() const override;
private:
    Amount price = 500;
};
