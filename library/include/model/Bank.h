#pragma once
#include "Amount.h"
#include "managers/AccountManager.h"
#include "managers/ClientManager.h"
#include "managers/TransferManager.h"

/**
 * Wyjątek "Bankrutwo" oznacza, że bank nie miał środków na obsługę
 * klientów. Po zgłoszeniu wyjątku niemożliwe jest funkcjonowanie aplikacji
 */
class Bankruptcy : public std::runtime_error {
public:
    explicit Bankruptcy(const std::string& msg) : std::runtime_error(msg) {}
};


/**
 * Wyjątek związany z operowaniem na niepoprawnym koncie
 */
class InvalidAccountError : public std::logic_error {
public:
    /**
     * Konstruktor
     * @param msg Wiadomość pozwalająca ocenić źródło błędu
     */
    explicit InvalidAccountError(const std::string& msg);
};

/**
 * Singleton gromadzący cały stan banku
 */
class Bank {
public:
    /**
     * @returns Instancja singletona
     */
    static Bank& getInstance();

    /**
     * @returns Bieżący stan konta banku
     */
    Amount getBalance() const;

    /**
     * Zmienia stan konta banku
     * @param value Nowy stan konta
     */
    void setBalance(Amount value);

    /**
     * Zwiększa stan konta banku o ustaloną kwotę
     * @param by Kwota
     */
    void increaseBalance(Amount by);

    /**
     * Zmniejsza stan konta banku o ustaloną kwotę
     * @param by Kwota
     * @throws Bankruptcy Kiedy bank nie ma pieniążków
     */
    void decreaseBalance(Amount by);

    /**
     * @returns Menedżer klientów
     */
    ClientManager& getClientManager();

    /**
     * @returns Menedżer przelewów
     */
    TransferManager& getTransferManager();

    /**
     * @returns Menedżer kont
     */
    AccountManager& getAccountManager();

    /**
     * Wczytuje stan wszystkich menedżerów z plików
     */
    void loadAll();

    /**
     * Zapisuje stan wszystkich menedżerów do plików
     */
    void saveAll();

    /**
     * Funkcja pośrednicząca przy otwarciu konta
     * @param clientID PESEL klienta
     * @param accountType Typ konta
     */
    void openAccount(std::string clientID, AccountType accountType);

    /**
     * Funkcja pośrednicząca w braniu kredytu
     * @param months Długość kredytu
     * @param amount Kwota kredytu
     * @param number Numer konta operacyjnego
     * @param clientID PESEL klienta
     */
    void takeLoan(int months, Amount amount, std::string number, std::string clientID);

    /**
     * Funkcja zwracająca wektor z kontami klienta
     * @param clientID PESEL klienta
     * @return Wektor z kontami
     */
    std::vector<std::shared_ptr<Account>> checkAccounts(std::string clientID);

    /**
     * Funkcja pośrednicząca w zamawianiu karty dla klienta
     * @param number Numer konta
     * @param cardType Typ karty
     */
    void orderNewCard(std::string number, CardType cardType);

    /**
     * Funkcja tworząca depozyt
     * @param number Numer konta
     * @param amount Kwota depozytu
     * @return Wartość logiczna czy operacja się powiodła
     */
    bool createDeposit(std::string number, Amount amount);

    /**
     * Funkcja kończąca depozyt
     * @param number Numer konta
     */
    void endDeposit(std::string number);

    /**
     * Funkcja zwracająca informację o kredytach klienta
     * @param clientID PESEL klienta
     * @return
     */
    std::string loanInfo(std::string clientID);

    /**
     * Funkcja zwracająca informacje o lokacie
     * @param number Numer konta
     * @return Informacje o lokacie
     */
    std::string depositInfo(std::string number);

    /**
     * Funkcja pozwalająca wykonać transakcje
     * @param number Konto nadawcy
     * @param number2 Konto odbiorcy
     * @param amount Kwota transakcji
     * @return Wartość logiczna czy transakcja się powiodła
     */
    bool transaction(std::string number, std::string number2, Amount amount);
private:
    Bank();
    ~Bank();

    Amount balance = 10000000;
    ClientManager clientManager;
    TransferManager transferManager;
    AccountManager accountManager;
};
