#include "model/InvestmentAccount.h"

#include "model/Bank.h"

InvestmentAccount::InvestmentAccount(
    const std::string &accountNumber, const std::string &id
)
    : Account(accountNumber, id)
{}

GoldError::GoldError(const std::string& msg)
    : std::logic_error(msg) {}

InvestmentAccount::InvestmentAccount(const json& source)
    : Account(source)
{
    if (!source.contains("goldCoins"))
        return;

    goldCoins = GoldCoins::fromJSON(source["goldCoins"]);
}

void InvestmentAccount::buyGold(int count) {
    if (goldCoins)
        throw GoldError{"You must first sell currently owned coins"};

    auto goldCoins = std::make_shared<GoldCoins>(count);

    Amount newBalance = getBalance();
    try {
        newBalance -= goldCoins->calculateValue();
    } catch (const InvalidAmountError& e) {
        throw NotEnoughMoney{"You do not have enough funds"};
    }

    setBalance(newBalance);

    this->goldCoins = std::move(goldCoins);
}

Amount InvestmentAccount::sellGold() {
    if (!goldCoins)
        throw GoldError{"You do not have gold coins on this account"};

    auto value = goldCoins->calculateValue();
    
    auto fee = value * 0.02;
    Bank::getInstance().increaseBalance(fee);

    auto earned = value - fee;
    setBalance(getBalance() + earned);
    goldCoins = nullptr;

    return earned;
}

std::string InvestmentAccount::toString() const {
    std::ostringstream oss;
    oss << "Investment Account " << Account::toString() << ". Gold coins: ";

    if (goldCoins)
        oss << goldCoins->toString();
    else
        oss << "none";

    return oss.str();
}

json InvestmentAccount::toJSON() const {
    json j = Account::toJSON();
    j["type"] = AccountType::Investment;
    if (goldCoins)
        j["goldCoins"] = goldCoins;
    return j;
}

AccountType InvestmentAccount::getType() const {
    return AccountType::Investment;
}

std::shared_ptr<GoldCoins> InvestmentAccount::getGoldCoins() const {
    return goldCoins;
}

Amount InvestmentAccount::getFee() {
    return transferFee;
}

std::shared_ptr<Account> InvestmentAccount::fromJSON(const json& source) {
    class Helper : public InvestmentAccount {
    public:
        explicit Helper(const json& source) : InvestmentAccount(source) {}
    };

    return std::make_shared<Helper>(source);
}
