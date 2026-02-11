#pragma once
#include <boost/date_time.hpp>

#include "Amount.h"
#include "Serializable.h"

namespace pt = boost::posix_time;

/**
 * Klasa reprezentująca pewną ilość złotych monet
 */
class GoldCoins : public Serializable {
public:
    /**
     * Konstruktor
     * @param count Liczba złotych monet
     * @param purchaseTime Czas zakupu
     */
    explicit GoldCoins(
        int count,
        pt::ptime purchaseTime = pt::second_clock::local_time()
    );

    /**
     * @returns Ilosc zlotych monet
     */
    int getCount() const;

    /**
     * @return Czas zakupu złotych monet
     */
    pt::ptime getPurchaseTime() const;

    /**
     * Oblicza obecną wartość złotych monet.
     * Cena złotej monety wzrasta o złotówkę co dobę.
     * @param date Czas, dla którego obliczyć wartość
     */
    Amount calculateValue(pt::ptime date = pt::second_clock::local_time());

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
     * Tworzy obiekt z danych w formacie JSON
     * @param source Informacje o obiekcie w formacie JSON
     * @returns Obiekt złotych monet
     */
    static std::shared_ptr<GoldCoins> fromJSON(const json& source);
private:
    int count;
    boost::posix_time::ptime purchaseTime;
};
