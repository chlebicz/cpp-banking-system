#pragma once
#include "Repository.h"
#include "StorageHandler.h"
#include "model/Account.h"
#include "model/InvestmentAccount.h"
#include "model/SavingsAccount.h"

/**
 * Account Manager
 */
class AccountManager {
public:
    /**
     * Opens an account of a given type for a client. If an account
     * of the given type cannot be created for the client, an object
     * of the existing account is returned.
     * @param clientID Client - account owner
     * @param accountType Type of account being created
     * @returns Object of the created/existing account
     */
    std::shared_ptr<Account> openAccount(
        std::string clientID, AccountType accountType
    );

    /**
     * Closes the account with the given account number.
     * @param accountID Account number
     * @returns True if the account was closed, otherwise false
     */
    bool closeAccount(const std::string& accountID);

    /**
     * @param accountID Account number
     * @returns Account with the given account number, nullptr if it does not exist.
     */
    std::shared_ptr<Account> getAccount(const std::string& accountID);

    /**
     * Finds all accounts of the client with the given PESEL number
     * @param clientID PESEL number
     * @return Vector with client account objects
     */
    std::vector<std::shared_ptr<Account>> findClientAccounts(
        const std::string& clientID
    );

    /**
     * Checks if the account with the given account number belongs to the client
     * with the given identifier
     * @param clientID Client identifier
     * @param accountID Account number
     * @returns True if the account belongs to the client, otherwise false
     */
    bool isClientsAccount(
        const std::string& clientID, const std::string& accountID
    );

    /**
     * Finds the first account satisfying the given predicate.
     * @param predicate Predicate
     * @returns Found account
     */
    std::shared_ptr<Account> findAccount(
        const Predicate<std::shared_ptr<Account>>& predicate
    );

    /**
     * Finds all accounts satisfying the given predicate.
     * @param predicate Predicate
     * @returns Vector with objects of found accounts
     */
    std::vector<std::shared_ptr<Account>> findAllAccounts(
        const Predicate<std::shared_ptr<Account>>& predicate
    );

    /**
     * Finds investment accounts belonging to the given client
     * @param ownerID Client identifier of the account owner
     * @returns Vector with investment account objects
     */
    std::vector<std::shared_ptr<InvestmentAccount>> findInvestmentAccounts(
        const std::string& ownerID
    );

    /**
     * Finds savings accounts belonging to the given client
     * @param ownerID Client identifier of the account owner
     * @returns Vector with savings account objects
     */
    std::vector<std::shared_ptr<SavingsAccount>> findSavingsAccounts(
        const std::string& ownerID
    );

    /**
     * @returns All accounts in the repository
     */
    const std::vector<std::shared_ptr<Account>>& getAllAccounts();

    /**
     * Saves account objects from the repository to files.
     */
    void save();

    /**
     * Loads account objects from files to the repository.
     */
    void load();
private:
    template<typename T>
    std::vector<std::shared_ptr<T>> findClientAccounts(
        const std::string& ownerID, AccountType type
    );

    Repository<std::shared_ptr<Account>> repository;
    StorageHandler storageHandler{"accounts"};
};

template<typename T>
std::vector<std::shared_ptr<T>> AccountManager::findClientAccounts(
    const std::string &ownerID, AccountType type
) {
    static_assert(std::is_base_of_v<Account, T>);

    auto accounts = repository.findAll([&ownerID, &type](const auto& acc) {
        bool ownerMatches = acc->getOwnerID() == ownerID;
        bool typeMatches = acc->getType() == type;
        return ownerMatches && typeMatches;
    });

    std::vector<std::shared_ptr<T>> result(accounts.size());
    std::transform(
        accounts.begin(), accounts.end(), result.begin(),
        [](const auto& acc) {
            return std::static_pointer_cast<T>(acc);
        }
    );

    return result;
}
