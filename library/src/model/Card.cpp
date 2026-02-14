#include "model/Card.h"

#include "model/DiamondCard.h"
#include "model/GoldCard.h"
#include "model/StandardCard.h"

json Card::toJSON() const {
    return {
        {"type", static_cast<int>(getType())}
    };
}

std::shared_ptr<Card> Card::fromJSON(const json& source) {
    unsigned int typeInt = std::stoi(source["type"].get<std::string>());
    if (typeInt > 2)
        throw std::invalid_argument("Invalid card type");

    auto type = static_cast<CardType>(typeInt);
    switch (type) {
        case CardType::Standard:
            return std::make_shared<StandardCard>();
        case CardType::Gold:
            return std::make_shared<GoldCard>();
        case CardType::Diamond:
            return std::make_shared<DiamondCard>();
    }

    throw std::runtime_error("Something went wrong...");
}
