#include "model/StandardCard.h"

StandardCard::StandardCard()
    : Card() {}

Amount StandardCard::calculateFee(const Amount &amount) const {
    Amount fee(amount.getZloty() * 0.05, amount.getGrosz() * 0.05);
    fee += baseFee;
    return fee;
}

CardType StandardCard::getType() const {
    return CardType::Standard;
}

std::string StandardCard::toString() const {
    return "Standard Card - additional fee of 5% of amount for each payment";
}

Amount StandardCard::getPrice() const {
    return price;
}
