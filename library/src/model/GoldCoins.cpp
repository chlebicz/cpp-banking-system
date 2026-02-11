#include "model/GoldCoins.h"

GoldCoins::GoldCoins(int count, pt::ptime purchaseTime)
    : count(count), purchaseTime(purchaseTime) {}

int GoldCoins::getCount() const {
    return count;
}

pt::ptime GoldCoins::getPurchaseTime() const {
    return purchaseTime;
}

Amount GoldCoins::calculateValue(
    pt::ptime date
) {
    if (date < purchaseTime)
        return -1;

    pt::time_period elapsed(purchaseTime, date);
    int days = elapsed.length().hours() / 24;

    return count * (1000 + days);
}

json GoldCoins::toJSON() const {
    return {
        {"count", count},
        {"purchaseTime", to_time_t(purchaseTime)}
    };
}

std::shared_ptr<GoldCoins> GoldCoins::fromJSON(const json& source) {
    return std::make_shared<GoldCoins>(
        source["count"],
        pt::from_time_t(source["purchaseTime"])
    );
}

std::string GoldCoins::toString() const {
    auto date = purchaseTime.date();

    std::ostringstream oss;
    oss << count << " zlotych monet, zakupione "
        << std::setfill('0') << std::setw(2) << date.day() << "."
        << std::setfill('0') << std::setw(2) << static_cast<int>(date.month())
        << "." << date.year();

    return oss.str();
}
