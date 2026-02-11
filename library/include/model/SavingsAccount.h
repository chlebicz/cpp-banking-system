#pragma once
#include "Account.h"
#include "Deposit.h"
#include <boost/date_time.hpp>

namespace gr = boost::gregorian;

/**
 * Konto oszczędnościowe
 */
class SavingsAccount : public Account {
public:
    /**
     * Konstruktor parametrowy
     * @param accountNumber Numer rachunku
     * @param id Identyfikator klienta posiadacza konta
     */
    SavingsAccount(const std::string &accountNumber, const std::string &id);

    /**
     * Funkcja tworząca lokatę
     * @param amount Kwota lokaty
     * @param beginTime Data rozpoczęcia lokaty
     */
    bool createDeposit(Amount amount, const gr::date &beginTime = gr::day_clock::local_day());

    /**
     * @returns Wskaźnik do obiektu lokaty (jeśli taka jest założona)
     */
    std::shared_ptr<Deposit> getDeposit() const;

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
     * Kończy lokatę
     */
    void deleteDeposit();

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
    /// Opłata od transferu
    Amount transferFee = 10;

    /// Wskaźnik na obiekt lokaty
    std::shared_ptr<Deposit> deposit = nullptr;

    /**
     * Konstruktor z JSONa
     * @param source Informacje o koncie w formacie JSON
     */
    explicit SavingsAccount(const json& source);
};
