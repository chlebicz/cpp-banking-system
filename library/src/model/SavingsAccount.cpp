#include "model/SavingsAccount.h"

/// Constructor
SavingsAccount::SavingsAccount(
    const std::string& accountNumber, const std::string& id
)
    : Account(accountNumber, id)
{}

SavingsAccount::SavingsAccount(const json& source)
    : Account(source)
{
    if (source.contains("deposit"))
        deposit = Deposit::fromJSON(source["deposit"]);
}

/// Function creating a deposit
/// Accepts 1 argument Amount
/// Returns bool
bool SavingsAccount::createDeposit(
    Amount amount, const gr::date &beginTime
) {
    // Case where deposit is already created
    if (deposit != nullptr) {
        return false;
    }

    // Case where there are not enough funds to create a deposit
    Amount newBalance = getBalance();
    try {
        newBalance -= amount;
    } catch (InvalidAmountError) {
        return false;
    }
    setBalance(newBalance);

    deposit = std::make_shared<Deposit>(amount, beginTime);
    return true;
}

/// Function returning a pointer to the deposit object
/// Accepts no arguments
std::shared_ptr<Deposit> SavingsAccount::getDeposit() const {
    return deposit;
}

/// Function returning object properties as a string
std::string SavingsAccount::toString() const {
    std::ostringstream oss;
    oss << "Savings Account " << Account::toString();

    if (deposit)
        oss << deposit->toString();

    return oss.str();
}

json SavingsAccount::toJSON() const {
    json j = Account::toJSON();
    j["type"] = AccountType::Savings;
    if (deposit)
        j["deposit"] = deposit;
    return j;
}

AccountType SavingsAccount::getType() const {
    return AccountType::Savings;
}

/// Function ending the deposit
/// Accepts no arguments
/// Returns nothing
void SavingsAccount::deleteDeposit() {
    if (deposit == nullptr) {
        return;
    }
    Amount profit = deposit->endDeposit();
    deposit = nullptr;
    setBalance(getBalance() + profit);
}

Amount SavingsAccount::getFee() {
    return transferFee;
}

std::shared_ptr<Account> SavingsAccount::fromJSON(const json& source) {
    class Helper : public SavingsAccount {
    public:
        explicit Helper(const json& source) : SavingsAccount(source) {}
    };

    return std::make_shared<Helper>(source);
}
