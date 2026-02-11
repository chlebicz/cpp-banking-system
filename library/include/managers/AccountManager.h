#pragma once
#include "Repository.h"
#include "StorageHandler.h"
#include "model/Account.h"
#include "model/InvestmentAccount.h"
#include "model/SavingsAccount.h"

/**
 * Menedżer kont
 */
class AccountManager {
public:
    /**
     * Otwiera konto o danym typie dla klienta. Jeśli nie można
     * już utworzyć konta danego typu dla klienta, to zwracany jest
     * obiekt istniejącego już konta.
     * @param clientID Klient - właściciel konta
     * @param accountType Typ tworzonego konta
     * @returns Obiekt utworzonego/istniejącego konta
     */
    std::shared_ptr<Account> openAccount(
        std::string clientID, AccountType accountType
    );

    /**
     * Zamyka konto o podanym numerze rachunku.
     * @param accountID Numer rachunku
     * @returns Prawda, jeśli konto zostało zamknięte, w przeciwnym wypadku fałsz
     */
    bool closeAccount(const std::string& accountID);

    /**
     * @param accountID Numer rachunku
     * @returns Konto o podanym numerze rachunku, nullptr jeśli nie istnieje.
     */
    std::shared_ptr<Account> getAccount(const std::string& accountID);

    /**
     * Wyszukuje wszystkie konta klienta o podanym numerze PESEL
     * @param clientID Numer PESEL
     * @return Wektor z obiektami kont klienta
     */
    std::vector<std::shared_ptr<Account>> findClientAccounts(
        const std::string& clientID
    );

    /**
     * Sprawdza czy konto o podanym numerze rachunku należy do klienta
     * o podanym identyfikatorze
     * @param clientID Identyfikator klienta
     * @param accountID Numer rachunku
     * @returns Prawda, jeśli konto należy do klienta, inaczej fałsz
     */
    bool isClientsAccount(
        const std::string& clientID, const std::string& accountID
    );

    /**
     * Wyszukuje pierwsze konto spełniające podany predykat.
     * @param predicate Predykat
     * @returns Znalezione konto
     */
    std::shared_ptr<Account> findAccount(
        const Predicate<std::shared_ptr<Account>>& predicate
    );

    /**
     * Wyszukuje wszystkie konta spełniające dany predykat.
     * @param predicate Predykat
     * @returns Wektor z obiektami znalezionych kont
     */
    std::vector<std::shared_ptr<Account>> findAllAccounts(
        const Predicate<std::shared_ptr<Account>>& predicate
    );

    /**
     * Wyszukuje konta inwestycyjne należące do podanego klienta
     * @param ownerID Identyfikator klienta właściciela kont
     * @returns Wektor z obiektami kont inwestycyjnych
     */
    std::vector<std::shared_ptr<InvestmentAccount>> findInvestmentAccounts(
        const std::string& ownerID
    );

    /**
     * Wyszukuje konta inwestycyjne należące do podanego klienta
     * @param ownerID Identyfikator klienta właściciela kont
     * @returns Wektor z obiektami kont inwestycyjnych
     */
    std::vector<std::shared_ptr<SavingsAccount>> findSavingsAccounts(
        const std::string& ownerID
    );

    /**
     * @returns Wszystkie konta w repozytorium
     */
    const std::vector<std::shared_ptr<Account>>& getAllAccounts();

    /**
     * Zapisuje obiekty kont z repozytorium do plików.
     */
    void save();

    /**
     * Ładuje obiekty kont z plików do repozytorium.
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
