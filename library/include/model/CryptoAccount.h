#pragma once
#include "Account.h"

class CryptoAccount : public Account {
public:
    /**
     * Konstruktor parametrowy
     * @param accountNumber Numer rachunku
     * @param id Identyfikator klienta posiadacza konta
     */
    CryptoAccount(const std::string &accountNumber, const std::string &id);

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
     * Funkcja zwracająca opłatę transferową
     * @return Opłata transferowa
     */
    Amount getFee() override;

    /**
     * Tworzy obiekt na bazie informacji w formacie JSON
     * @param source Informacje o obiekcie w formacie JSON
     * @returns Utworzony obiekt
     */
    static std::shared_ptr<Account> fromJSON(const json& source);

    /// Destruktor
    ~CryptoAccount() override = default;
private:
    /// Opłata transakcyjna
    Amount transferFee = 100;

    /**
     * Konstruktor z JSONa
     * @param source Informacje o koncie w formacie JSON
     */
    explicit CryptoAccount(const json& source);
};
