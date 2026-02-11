#include "model/SavingsAccount.h"

/// Konstruktor
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

/// Funkcja tworząca lokatę
/// Przyjmuje 1 argument Amount
/// Zwraca bool
bool SavingsAccount::createDeposit(
    Amount amount, const gr::date &beginTime
) {
    // Przypadek kiedy lokata jest już utworzona
    if (deposit != nullptr) {
        return false;
    }

    // Przypadek kiedy nie ma wystarczająco środków by utworzyć lokatę
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

/// Funkcja zwracająca wskaźnik do obiektu lokaty
/// Nie przyjmuje argumentów
std::shared_ptr<Deposit> SavingsAccount::getDeposit() const {
    return deposit;
}

/// Funkcja zwracająca właściwości obiektu jako string
std::string SavingsAccount::toString() const {
    std::ostringstream oss;
    oss << "Konto oszczednosciowe " << Account::toString();

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

/// Funkcja kończąca lokatę
/// Nie przyjmuje argumentów
/// Nic nie zwraca
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

