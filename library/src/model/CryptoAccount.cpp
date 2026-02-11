#include "model/CryptoAccount.h"

/// Konstruktor
CryptoAccount::CryptoAccount(
    const std::string &accountNumber, const std::string &id
)
    : Account(accountNumber, id) {
}

CryptoAccount::CryptoAccount(const json& source)
    : Account(source)
{}

std::string CryptoAccount::toString() const {
    return "Konto kryptowalutowe " + Account::toString();
}

json CryptoAccount::toJSON() const {
    json j = Account::toJSON();
    j["type"] = AccountType::Crypto;
    return j;
}

AccountType CryptoAccount::getType() const {
    return AccountType::Crypto;
}

Amount CryptoAccount::getFee() {
    return transferFee;
}

std::shared_ptr<Account> CryptoAccount::fromJSON(const json& source) {
    class Helper : public CryptoAccount {
    public:
        explicit Helper(const json& source) : CryptoAccount(source) {}
    };

    return std::make_shared<Helper>(source);
}
