#include "model/DiamondCard.h"

DiamondCard::DiamondCard()
    : Card()
{}

Amount DiamondCard::calculateFee(const Amount &amount) const {
    Amount fee = 0;
    fee = fee + baseFee;
    return fee;
}

CardType DiamondCard::getType() const {
    return CardType::Diamond;
}

std::string DiamondCard::toString() const {
    return "Karta Diamond - brak oplat dodatkowych";
}

Amount DiamondCard::getPrice() const {
    return price;
}
