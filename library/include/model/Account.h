#pragma once
#include <string>
#include "Amount.h"
#include "Card.h"
#include <memory>
#include <vector>

#include "Entity.h"
#include "Serializable.h"

/**
 * Typ konta
 */
enum class AccountType {
    Main,
    Savings,
    Investment,
    Crypto
};

/**
 * Wyjątek zgłaszany kiedy na koncie nie ma środków by wykonać
 * daną operację
 */
class NotEnoughMoney : public std::logic_error {
public:
    /**
     * Konstruktor
     * @param msg Wiadomość pozwalająca ocenić źródło błędu
     */
    explicit NotEnoughMoney(const std::string& msg)
        : std::logic_error(msg) {}
};

/**
 * Konto
 */
class Account : public Entity {
public:
    /**
     * Konstruktor parametrowy
     * @param accountNumber Numer rachunku
     * @param ownerID Identyfikator klienta posiadacza konta
     */
    Account(std::string accountNumber, std::string ownerID);

    /**
     * Konstruktor z danych z pliku
     * @param accountNumber Numer rachunku
     * @param ownerID Identyfikator klienta posiadacza konta
     * @param balance Stan konta
     * @param cards Wektor kard przypisanych do konta
     */
    Account(
        std::string accountNumber, std::string ownerID, Amount balance,
        std::vector<std::shared_ptr<Card>> cards
    );

    /**
     * Getter salda
     * @returns Zwraca stan konta
     */
    Amount getBalance() const;

    /**
     * Setter salda
     * @param value Kwota na jaką chcemy ustawić saldo
     */
    void setBalance(const Amount &value);

    /**
     * Getter numeru konta
     * @returns Zwraca numer konta
     */
    const std::string& getAccountNumber() const;

    /**
     * Getter PESELu właściciela konta
     * @returns Zwraca PESEL
     */
    const std::string& getOwnerID() const;

    /**
    * Getter kart przypisanych do konta
    * @returns Zwraca wektor z kartami
    */
    const std::vector<std::shared_ptr<Card>>& getCards() const;

    /**
     * Dodawanie kart do konta
     * @param card Karta którą chcemy przypisać do konta
     */
    void addCard(const std::shared_ptr<Card> &card);

    /**
     * Usuwanie kart z konta
     * @param card Karta którą chcemy usunąć z konta
     */
    void removeCard(const std::shared_ptr<Card> &card);

    /**
     * Funkcja wykonująca transakcje
     * @param amount Kwota transakcji
     * @param account Konto na które wykonujemy transakcje
     */
    bool transaction(const Amount &amount, const std::shared_ptr<Account> &account);

    /**
     * @return Istotne informacje o danym obiekcie zapisane w formacie
     *         przyjaznym użytkownikowi
     */
    std::string toString() const override;

    /**
     * @returns Unikalny identyfikator obiektu
     */
    std::string getId() const override;

    /**
     * @returns Wszystkie informacje o danym obiekcie w formacie JSON, które
     *          są potrzebne do otworzenia jego stanu przy odczycie z pliku
     */
    json toJSON() const override;

    /**
     * @returns Typ konta
     */
    virtual AccountType getType() const = 0;

    /**
     * Tworzy obiekt (uwzględniając odpowiedni typ konta) na bazie informacji
     * w formacie JSON
     * @param source Informacje o obiekcie w formacie JSON
     * @returns Utworzony obiekt
     */
    static std::shared_ptr<Account> fromJSON(const json& source);

    /**
     * Funkcja zwracające opłatę za transfery z użyciem konta
     * @return Zwraca opłatę transferową
     */
    virtual Amount getFee();
private:
    std::string accountNumber, ownerID;
    Amount transferFee = 0;
    Amount balance = 0;
    std::vector<std::shared_ptr<Card>> cards;
protected:
    /**
     * Konstruktor z JSONa
     * @param source Informacje o koncie w formacie JSON
     */
    explicit Account(const json& source);
};
