#pragma once
#include "Account.h"
#include "Amount.h"
#include <boost/date_time.hpp>

namespace gr = boost::gregorian;

/**
 * Kredyt
 */
class Loan : public Serializable {
public:
    /**
     * Konstruktor parametrowy
     * @param months Czas trwania kredytu
     * @param amount Kwota kredytu
     * @param beginTime Czas rozpoczęcia kredytu
     * @param operationalAccount Konto na którym są obsługiwane
     *                                 przelewy związane z kredytem
     */
    Loan(
        int months, Amount amount, gr::date beginTime,
        std::shared_ptr<Account> operationalAccount
    );

    /**
     * Konstruktor parametrowy
     * @param months Czas trwania kredytu
     * @param amount Kwota kredytu
     * @param beginTime Czas rozpoczęcia kredytu
     * @param operationalAccountNumber Konto na którym są obsługiwane
     *                                 przelewy związane z kredytem
     */
    Loan(
        int months, Amount amount, gr::date beginTime,
        std::string operationalAccountNumber
    );

    /**
     * Konstruktor parametrowy
     * @param months Czas trwania kredytu
     * @param amount Kwota kredytu
     * @param beginTime Czas rozpoczęcia kredytu
     * @param operationalAccountNumber Konto na którym są obsługiwane
     *                                 przelewy związane z kredytem
     * @param monthlyPayment Rata kredytu
     */
    Loan(
        int months, Amount amount, gr::date beginTime,
        std::string operationalAccountNumber, Amount monthlyPayment
    );

    /**
     * Zaciąga kredyt
     */
    void create();

    /**
     * Getter długości kredytu
     * @return Zwraca ilość miesięcy na jakie został wzięty kredyt
     */
    int getMonths() const;

    /**
     * Getter kwoty kredytu
     * @return Zwraca kwotę kredytu
     */
    Amount getAmount() const;

    /**
     * Getter miesięcznej opłaty za kredyt
     * @return Zwraca kwotę miesięcznej opłaty za kredyt
     */
    Amount getMontlyPayment() const;

    /**
     * Getter pozostałych miesięcy
     * @return Zwraca ilość miesięcy pozostałych do spłaty kredytu
     */
    int getRemainingMonths() const;

    /**
     * Getter dnia rozpoczęcia kredytu
     * @return Zwraca dzień rozpoczęcia kredytu
     */
    gr::date getBeginTime() const;

    /**
     * Getter konta operacyjnego
     * @return Konto operacyjne
     */
    std::shared_ptr<Account> getOperationalAccount() const;

    /**
     * Obciąża konto klienta ratą kredytu
     * @returns Czy udało się obciążyć konto
     */
    bool takeMoney();

    /**
     * Funkcja zwracająca informacje o kredycie jako string
     * @return Zwraca informacje o kredycie
     */
    std::string toString() const override;

    /**
     * @returns Wszystkie informacje o danym obiekcie w formacie JSON, które
     *          są potrzebne do otworzenia jego stanu przy odczycie z pliku
     */
    json toJSON() const override;

    /**
     * Tworzy obiekt z danych w formacie JSON
     * @param source Informacje o obiekcie w formacie JSON
     * @returns Obiekt złotych monet
     */
    static std::shared_ptr<Loan> fromJSON(const json& source);
private:
    int months;
    int paidMonths = 0;
    Amount amount;
    Amount monthlyPayment = 0;
    std::string operationalAccountNumber;
    std::shared_ptr<Account> operationalAccount;

    /// Dzień rozpoczęcia kredytu
    gr::date beginTime;
};
