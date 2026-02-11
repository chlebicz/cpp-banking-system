#pragma once
#include <string>

/**
 * Generator losowych danych
 */
class RandomDataGenerator {
public:
    /**
     * @return Losowy numer rachunku w formacie IBAN
     */
    static std::string generateIBAN();

    /**
     * @returns Losowy identyfikator
     */
    static std::string generateID();
private:
    static std::string generateStringWithDigits(unsigned int length);
};
