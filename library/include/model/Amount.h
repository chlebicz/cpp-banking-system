#pragma once
#include <stdexcept>
#include <string>

#include "Serializable.h"

/**
 * Bład związany z niepoprawną kwotą - wyrzucany np. przy złym formacie kwoty
 * lub jeśli przy odejmowaniu wynik jest mniejszy od zera.
 */
class InvalidAmountError : public std::logic_error {
public:
    /**
     * Konstruktor
     * @param msg Wiadomość pozwalająca ocenić źródło błędu
     */
    explicit InvalidAmountError(const std::string& msg);
};

/**
 * Klasa reprezentująca każdą kwotę obsługiwaną przez bank, przechowuje
 * dane w postaci całkowitej - część złotowa i groszowa kwoty
 */
class Amount : public Serializable {
public:
    /**
     * Konstruktor kwoty z części złotowej i groszowej
     * @param zloty Część złotowa
     * @param grosz Część groszowa (jeśli >= 100 to zostanie przeliczona
     *              na odpowiednią ilość złotówek i groszy)
     */
    Amount(unsigned int zloty, unsigned int grosz);

    /**
     * Konstruktor kwoty z postaci zmiennoprzecinkowej, tworzy kwotę
     * przybliżając podaną liczbę do dwóch cyfr po przecinku w dół
     * @param fromFloat Liczba zmiennoprzecinkowa reprezentująca kwotę
     */
    Amount(float fromFloat);

    /**
     * Metoda wytwórcza kwoty z podanego ciągu znaków.
     * @param str Ciąg znaków reprezentujący kwotę w formacie "...(,..) (zł)"
     * @throws InvalidAmountError Jeżeli kwota została podana w niepoprawnym
     *                            formacie
     */
    static Amount fromString(const std::string& str);

    /**
     * Metoda wytwórcza kwoty z formatu JSON (string)
     * @param j Obiekt JSON reprezentujący kwotę w formacie stringa
     * @throws InvalidAmountError Z metody Amount::fromString
     */
    static Amount fromJSON(const json& j);

    json toJSON() const override;

    /**
     * @returns Część złotowa kwoty
     */
    unsigned int getZloty() const;

    /**
     * @returns Część groszowa kwoty
     */
    unsigned int getGrosz() const;

    /**
     * @return Reprezentacja kwoty w postaci ciągu znaków - format "...,..zł"
     */
    std::string toString() const;

    /**
     * Mnożenie kwoty przez liczbę zmiennoprzecinkową (np. do obliczania
     * danego procentu kwoty). Wynik może być niedokładny.
     * @param right Mnożnik
     * @returns Pomnożona kwota
     */
    Amount operator*(float right) const;

    /**
     * Przeładowanie operatora dodawania dla kwot
     * @param right Kwota po prawej stronie znaku "+"
     * @return Suma kwoty na której ta metoda została wywołana i kwoty
     *         podanej jako argument
     */
    Amount operator+(const Amount& right) const;

    /**
     * Przeladowanie operatora przypisania z dodawaniem dla kwot
     * @param right Kwota po prawej stronie operatora "+="
     */
    void operator+=(const Amount& right);

    /**
     * Przeladowanie operatora przypisania z odejmowaniem dla kwot
     * @param right Kwota po prawej stronie operatora "-="
     * @throws InvalidAmountError Zgłasza wyjątek, kiedy odejmowana kwota jest
     *                            większa od kwoty, na której wywołano operator.
     */
    void operator-=(const Amount& right);

    /**
     * Przeładowanie operatora odejmowania dla kwot
     * @param right Kwota po prawej stronie operatora "-"
     * @throws InvalidAmountError Zgłasza wyjątek jeśli kwota odejmowana jest
     *                            większa niż kwota, na której wywołano operator.
     * @returns Różnica kwoty na której ta metoda została wywołana i kwoty
     *          podanej jako argument
     */
    Amount operator-(const Amount& right) const;

    /**
     * @param right Kwota po prawej stronie operatora
     * @return czy kwoty są równe
     */
    bool operator==(const Amount& right) const;

    /**
     * @param right Kwota po prawej stronie operatora
     * @return czy kwoty są różne
     */
    bool operator!=(const Amount& right) const;

    /**
     * @param right Kwota po prawej stronie operatora
     * @return czy kwota po prawej jest większa od kwoty po lewej
     */
    bool operator<(const Amount& right) const;

    /**
     * @param right Kwota po prawej stronie operatora
     * @return czy kwota po lewej jest większa od kwoty po prawej
     */
    bool operator>(const Amount& right) const;

    /**
     * @param right Kwota po prawej stronie operatora
     * @return czy kwota po prawej jest większa lub równa kwocie po lewej
     */
    bool operator<=(const Amount& right) const;

    /**
     * @param right Kwota po prawej stronie operatora
     * @return czy kwota po lewej jest większa lub równa kwocie po prawej
     */
    bool operator>=(const Amount& right) const;
private:
    unsigned int zloty, grosz;
};

/**
 * Umożliwia wypisanie kwoty na strumień
 * Wymagane do poprawnego kompilowania testów boost
 * @param os Strumień wyjściowy
 * @param amount Wypisywana kwota
 * @return Zmieniony strumień
 */
std::ostream& operator<<(std::ostream& os, const Amount& amount);
