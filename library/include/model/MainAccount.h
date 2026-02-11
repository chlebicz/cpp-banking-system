#pragma once
#include "Account.h"

/**
 * Konto główne
 */
class MainAccount : public Account {
public:
    /**
     * Konstruktor parametrowy
     * @param accountNumber Numer rachunku
     * @param id Identyfikator klienta posiadacza konta
     */
    MainAccount(const std::string &accountNumber, const std::string &id);

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
     * Tworzy obiekt (uwzględniając odpowiedni typ konta) na bazie informacji
     * w formacie JSON
     * @param source Informacje o obiekcie w formacie JSON
     * @returns Utworzony obiekt
     */
    static std::shared_ptr<Account> fromJSON(const json& source);
private:
    /// Opłata transferowa
    Amount transferFee = 0;

    /**
     * Konstruktor z JSONa
     * @param source Informacje o koncie w formacie JSON
     */
    explicit MainAccount(const json& source);
};
