#pragma once
#include "Repository.h"
#include "StorageHandler.h"
#include "model/Transfer.h"

class ClientManager;
class AccountManager;

/**
 * Wyjątek zgłaszany kiedy nie uda się wykonać przelewu o podanych parameterach
 */
class InvalidTransferError : public std::logic_error {
public:
    /**
     * Konstruktor
     * @param msg Wiadomość pozwalająca ocenić źródło błędu
     */
    explicit InvalidTransferError(const std::string& msg)
        : std::logic_error(msg) {}
};

/**
 * Menedżer przelewów
 */
class TransferManager {
public:
    /**
     * Konstruktor
     * @param clientManager Menedżer klientów
     * @param accountManager Menedżer kont
     */
    TransferManager(
        ClientManager& clientManager, AccountManager& accountManager
    );

    /**
     * Rejestruje przelew o podanych parametrach
     * @param senderID Numer rachunku konta, z którego poszedł przelew
     * @param recipientID Numer rachunku odbiorcy
     * @param amount Kwota przelewu
     * @param type Typ przelewu
     * @returns Obiekt utworzonego przelewu
     * @throws NotEnoughMoney Kiedy na koncie nie ma wystarczająco środków
     * @throws InvalidTransferError Kiedy przelew ma niepoprawny typ
     */
    std::shared_ptr<Transfer> createTransfer(
        std::string senderID, std::string recipientID, Amount amount
    );

    /**
     * @param transferID Identyfikator przelewu
     * @returns Obiekt przelewu o podanym identyfikatorze
     */
    std::shared_ptr<Transfer> getTransfer(const std::string& transferID);

    /**
     * Znajduje wszystkie przelewy (wychodzące i przychodzące), które
     * dotyczą danego użytkownika.
     * @param personalId Numer PESEL klienta
     * @returns Wektor z obiektami znalezionych przelewów
     */
    std::vector<std::shared_ptr<Transfer>> findUserTransfers(
        const std::string& personalId
    );

    /**
     * Znajduje wszystkie przelewy (wychodzące i przychodzące), które
     * dotyczą danego rachunku.
     * @param accountNumber Numer rachunku konta
     * @returns Wektor z obiektami znalezionych przelewów
     */
    std::vector<std::shared_ptr<Transfer>> findAccountTransfers(
        const std::string& accountNumber
    );

    /**
     * Znajduje pierwszy przelew spełniający podany predykat
     * @param predicate Predykat
     * @returns Obiekt znalezionego przelewu
     */
    std::shared_ptr<Transfer> findFirst(
        const Predicate<std::shared_ptr<Transfer>>& predicate
    );

    /**
     * Znajduje wszystkie przelewy spełniające podany predykat
     * @param predicate Predykat
     * @returns Wektor z obiektami znalezionych przelewów
     */
    std::vector<std::shared_ptr<Transfer>> findAll(
        const Predicate<std::shared_ptr<Transfer>>& predicate
    );

    /**
     * @returns Wszystkie obiekty przelewów
     */
    const std::vector<std::shared_ptr<Transfer>>& getAll();

    /**
     * Ładuje obiekty przelewów z plików do repozytorium
     */
    void load();

    /**
     * Zapisuje obiekty przelewów z repozytorium do plików
     */
    void save();

    /**
     * Obsługuje przychodzące przelewy zewnętrzne, których dane zostały
     * zapisane w odpowiednich plikach.
     */
    void handleIncomingExternalTransfers();
private:
    ClientManager& clientManager;
    AccountManager& accountManager;

    Repository<std::shared_ptr<Transfer>> repository;
    StorageHandler storageHandler{"transfers"};
};
