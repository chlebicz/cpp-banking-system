#pragma once
#include "Card.h"

/**
 * Karta Gold
 */
class GoldCard : public Card {
public:
    /**
     * Konstruktor bezparametrowy
     */
    GoldCard();

    /**
     * Metoda obliczająca opłatę od transakcji kartą
     * @param amount Kwota od której ma zostać obliczona prowizja
     */
    Amount calculateFee(const Amount &amount) const override;

    /**
     * @returns Typ danej karty
     */
    CardType getType() const override;

    /**
     * @return Istotne informacje o danym obiekcie zapisane w formacie
     *         przyjaznym użytkownikowi
     */
    std::string toString() const override;

    /**
     * Metoda zwracająca cenę karty
     * @return Cena karty
     */
    Amount getPrice() const;
private:
    Amount price = 100;
};
