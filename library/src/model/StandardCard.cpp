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
    return "Karta Standard - przy kazdej platnosci oplata dodatkowa 5% kwoty";
}

Amount StandardCard::getPrice() const {
    return price;
}
