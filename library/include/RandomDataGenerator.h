#pragma once
#include <string>

/**
 * Random data generator
 */
class RandomDataGenerator {
public:
    /**
     * @return Random account number in IBAN format
     */
    static std::string generateIBAN();

    /**
     * @returns Random identifier
     */
    static std::string generateID();
private:
    static std::string generateStringWithDigits(unsigned int length);
};
