#include "model/Transfer.h"

Transfer::Transfer(
    std::string id, std::string senderID, std::string recipientID,
    Amount amount, Amount additionalFee, TransferType type
)
    : id(std::move(id)), senderID(std::move(senderID)),
      recipientID(std::move(recipientID)), amount(amount),
      additionalFee(additionalFee), type(type)
{}

TransferType Transfer::getType() const {
    return type;
}

bool Transfer::concernsAccount(const std::string& accountNumber) const {
    return senderID == accountNumber || recipientID == accountNumber;
}

Amount Transfer::getAmount() const {
    return amount;
}

Amount Transfer::getFee() const {
    if (amount <= additionalFee)
        return 0;

    if (type != TransferType::OutcomingExternal)
        return additionalFee;

    static const Amount externalTransferFee = {0,12};
    auto totalFee = additionalFee + externalTransferFee;
    if (amount <= totalFee)
        return additionalFee;

    return totalFee;
}

Amount Transfer::getSentAmount() const {
    return amount + getFee();
}

Amount Transfer::getReceivedAmount() const {
    return amount;
}

const std::string& Transfer::getSenderID() const {
    return senderID;
}

const std::string& Transfer::getRecipientID() const {
    return recipientID;
}

std::string Transfer::getId() const {
    return id;
}

std::string Transfer::toString() const {
    return id + ";" + senderID + ";" + recipientID + ";" + amount.toString()
        + ";" + std::to_string(static_cast<int>(type)) + ";";
}

json Transfer::toJSON() const {
    return {
        {"id", id},
        {"senderID", senderID},
        {"recipientID", recipientID},
        {"amount", amount},
        {"additionalFee", additionalFee},
        {"type", static_cast<int>(type)}
    };
}

std::shared_ptr<Transfer> Transfer::fromJSON(const json& source) {
    // obecnie ta metoda zakłada, że podany obiekt JSON ma poprawny format

    return std::make_shared<Transfer>(
        source["id"],
        source["senderID"],
        source["recipientID"],
        Amount::fromJSON(source["amount"]),
        Amount::fromJSON(source["additionalFee"]),
        static_cast<TransferType>(source["type"])
    );
}
