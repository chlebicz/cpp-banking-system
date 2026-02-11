#pragma once
#include <memory>
#include "Amount.h"
#include "Serializable.h"

/**
 * Typ karty
 */
enum class CardType {
    Standard = 0,
    Gold = 1,
    Diamond = 2
};

/**
 * Karta
 */
class Card : public Serializable {
public:
    /**
     * Metoda obliczająca opłatę od transakcji kartą
     * @param amount Kwota od której ma zostać obliczona prowizja
     */
    virtual Amount calculateFee(const Amount& amount) const = 0;

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
    static std::shared_ptr<Card> fromJSON(const json& source);

    /**
     * @returns Typ danej karty
     */
    virtual CardType getType() const = 0;

    /**
     * Metoda zwracająca cenę karty
     * @return Cena karty
     */
    virtual Amount getPrice() const = 0;
protected:
    /// Podstawowa opłata od transakcji
    Amount baseFee = 2;
};
