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
    return "Gold Card - additional fee of 3% of amount for each payment";
}

Amount GoldCard::getPrice() const {
    return price;
}
