#pragma once
#include "Account.h"
#include "GoldCoins.h"

/**
 * Wyjątek zgłaszany przy błędach z kupnem/sprzedażą złota
 */
class GoldError : public std::logic_error {
public:
    explicit GoldError(const std::string& msg);
};

/**
 * Konto inwestycyjne
 */
class InvestmentAccount : public Account {
public:
    /**
     * Konstruktor parametrowy
     * @param accountNumber Numer rachunku
     * @param id Identyfikator klienta posiadacza konta
     */
    InvestmentAccount(const std::string &accountNumber, const std::string &id);

    /**
     * Funkcja pozwalająca na zakup złotych monet
     * @param count Ilość złotych monet do zakupu
     */
    void buyGold(int count);

    /**
     * Funkcja pozwalająca na sprzedaż złotych monet
     * @returns Zarobiona kwota
     */
    Amount sellGold();

    /**
     * Funkcja zwracająca właściwości obiektu jako string
     * @return Zwraca informacje o koncie
     */
    std::string toString() const override;

    /**
     * @returns Wszystkie informacje o danym obiekcie w formacie JSON, które
     *          są potrzebne do otworzenia jego stanu przy odczycie z pliku
     */
    json toJSON() const override;

    /**
     * @returns Typ konta
     */
    AccountType getType() const override;

    /**
     * @returns Obiekt złotych monet jeśli są takie przypisane do konta
     */
    std::shared_ptr<GoldCoins> getGoldCoins() const;

    /**
     * Funkcja zwracająca opłatę transferową
     * @return Opłata transferowa
     */
    Amount getFee() override;

    /**
     * Tworzy obiekt (uwzględniając odpowiedni typ konta) na bazie informacji
     * w formacie JSON
     * @param source Informacje o obiekcie w formacie JSON
     * @returns Utworzony obiekt
     */
    static std::shared_ptr<Account> fromJSON(const json& source);
private:
    // złote monety
    std::shared_ptr<GoldCoins> goldCoins;

    /// Opłata od transferu
    Amount transferFee = 5;

    /**
     * Konstruktor z JSONa
     * @param source Informacje o koncie w formacie JSON
     */
    explicit InvestmentAccount(const json& source);
};
