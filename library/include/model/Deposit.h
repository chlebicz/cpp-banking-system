#pragma once
#include "Amount.h"
#include <boost/date_time.hpp>

namespace gr = boost::gregorian;

class Deposit : public Serializable {
public:
    /**
     * Konstruktor parametrowy
     * @param amount Kwota lokaty
     * @param beginTime Data rozpoczęcia lokaty
     */
    Deposit(const Amount &amount, const gr::date &beginTime);

    /**
     * Getter kwoty lokaty
     * @returns Zwraca kwotę lokaty
     */
    Amount getAmount() const;

    /**
     * Getter dnia rozpoczęcia lokaty
     * @returns Zwraca dzień rozpoczęcia lokaty
     */
    gr::date getBeginTime() const;

    /**
     * Funkcja kończąca lokatę, oblicza kwotę jaka powinna zostać zwrócona na konto
     * @returns Zwraca kwotę lokaty powiększoną o odsetki
     */
    Amount endDeposit();

    /**
     * Funkcja zwracająca właściwości obiektu jako string
     * @returns Zwraca dane lokaty
     */
    std::string toString() const override;

    /**
     * @returns Wszystkie informacje o danym obiekcie w formacie JSON, które
     *          są potrzebne do otworzenia jego stanu przy odczycie z pliku
     */
    json toJSON() const override;

    /**
     * Tworzy obiekt na bazie informacji w formacie JSON
     * @param source Informacje o obiekcie w formacie JSON
     * @returns Utworzony obiekt
     */
    static std::shared_ptr<Deposit> fromJSON(const json& j);
private:
    /// Kwota lokaty
    Amount amount;

    /// Dzień rozpoczęcia lokaty
    gr::date beginTime;
};
