#include "model/MainAccount.h"

/// Constructor
MainAccount::MainAccount(const std::string &accountNumber, const std::string &id)
    : Account(accountNumber, id) {
}

MainAccount::MainAccount(const json& source)
    : Account(source)
{}

std::string MainAccount::toString() const {
    return "Main Account " + Account::toString();
}

json MainAccount::toJSON() const {
    json j = Account::toJSON();
    j["type"] = AccountType::Main;
    return j;
}

AccountType MainAccount::getType() const {
    return AccountType::Main;
}

Amount MainAccount::getFee() {
    return transferFee;
}

std::shared_ptr<Account> MainAccount::fromJSON(const json& source) {
    class Helper : public MainAccount {
    public:
        explicit Helper(const json& source) : MainAccount(source) {}
    };

    return std::make_shared<Helper>(source);
}
