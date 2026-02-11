#include "model/GoldCard.h"

GoldCard::GoldCard() = default;

Amount GoldCard::calculateFee(const Amount &amount) const {
    Amount fee(amount.getZloty() * 0.03, amount.getGrosz() * 0.03);
    fee = fee + baseFee;
    return fee;
}

CardType GoldCard::getType() const {
    return CardType::Gold;
}

std::string GoldCard::toString() const {
    return "Karta Gold - przy kazdej platnosci oplata dodatkowa 3% kwoty";
}

Amount GoldCard::getPrice() const {
    return price;
}
