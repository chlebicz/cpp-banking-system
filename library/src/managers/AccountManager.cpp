#include "managers/AccountManager.h"

#include "RandomDataGenerator.h"
#include "model/CryptoAccount.h"
#include "model/InvestmentAccount.h"
#include "model/MainAccount.h"
#include "model/SavingsAccount.h"

std::shared_ptr<Account> AccountManager::openAccount(
    std::string clientID, AccountType accountType
) {
    auto accountNumber = RandomDataGenerator::generateIBAN();

    std::shared_ptr<Account> account;
    switch (accountType) {
        case AccountType::Main:
            account = std::make_shared<MainAccount>(accountNumber, clientID);
            break;
        case AccountType::Crypto:
            account = std::make_shared<CryptoAccount>(accountNumber, clientID);
            break;
        case AccountType::Investment:
            account = std::make_shared<InvestmentAccount>(accountNumber, clientID);
            break;
        case AccountType::Savings:
            account = std::make_shared<SavingsAccount>(accountNumber, clientID);
            break;
    }

    repository.add(account);
    return account;
}

bool AccountManager::closeAccount(const std::string& accountID) {
    auto account = getAccount(accountID);
    if (!account)
        return false;

    repository.remove(account);
    return true;
}

std::shared_ptr<Account> AccountManager::getAccount(const std::string& accountID) {
    return repository.findFirst(
        [&accountID](const std::shared_ptr<Account>& account) {
            return account->getId() == accountID;
        }
    );
}

std::vector<std::shared_ptr<Account>> AccountManager::findClientAccounts(
    const std::string& clientID
) {
    return repository.findAll(
        [&clientID](const std::shared_ptr<Account>& account) {
            return account->getOwnerID() == clientID;
        }
    );
}

bool AccountManager::isClientsAccount(
    const std::string& clientID, const std::string& accountID
) {
    auto clientAccounts = findClientAccounts(clientID);
    auto accountIterator = std::find_if(
        clientAccounts.begin(), clientAccounts.end(),
        [&accountID] (const auto& account) {
            return account->getId() == accountID;
        }
    );
    return accountIterator != clientAccounts.end();
}

std::shared_ptr<Account> AccountManager::findAccount(
    const Predicate<std::shared_ptr<Account>>& predicate
) {
    return repository.findFirst(predicate);
}

std::vector<std::shared_ptr<Account>> AccountManager::findAllAccounts(
    const Predicate<std::shared_ptr<Account>> &predicate
) {
    return repository.findAll(predicate);
}

std::vector<std::shared_ptr<InvestmentAccount>>
AccountManager::findInvestmentAccounts(const std::string& ownerID) {
    return findClientAccounts<InvestmentAccount>(
        ownerID, AccountType::Investment
    );
}

std::vector<std::shared_ptr<SavingsAccount>>
AccountManager::findSavingsAccounts(const std::string& ownerID) {
    return findClientAccounts<SavingsAccount>(
        ownerID, AccountType::Savings
    );
}

const std::vector<std::shared_ptr<Account>>& AccountManager::getAllAccounts() {
    return repository.getAll();
}

void AccountManager::save() {
    // Najpierw usuwamy wszystkie pliki (aby pliki usuniętych podczas
    // działania programu obiektów zostały usunięte)
    storageHandler.removeAll();

    for (const auto& account : repository.getAll())
        storageHandler.saveObject(account);
}

void AccountManager::load() {
    repository.removeAll();

    auto objects = storageHandler.getAllObjects();
    for (const auto& [id, data] : objects) {
        auto account = Account::fromJSON(data);
        repository.add(account);
    }
}
