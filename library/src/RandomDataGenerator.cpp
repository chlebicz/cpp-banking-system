#include "RandomDataGenerator.h"

#include <random>
#include <sstream>

std::string RandomDataGenerator::generateIBAN() {
    static const std::string bankId = "12345678";

    std::string bban = bankId + generateStringWithDigits(16);

    // Numer konta z przeniesionymi 4 pierwszymi znakami na koniec,
    // a następnie zamienionymi znakami na odpowiednie ciągi cyfr,
    // A -> "10", B -> "11", ... PL -> "2521"
    std::string ibanShifted = bban + "252100";

    // zoptymalizowane modulo int(ibanShifted) % 97
    int mod = 0;
    for (char c : ibanShifted) {
        int currentNum = c - '0';
        mod = (10 * mod + currentNum) % 97;
    }

    int checksum = 98 - mod;

    auto checksumStr = std::to_string(checksum);
    if (checksumStr.length() == 1)
        checksumStr = "0" + checksumStr;

    return "PL" + checksumStr + bban;
}

std::string RandomDataGenerator::generateID() {
    return generateStringWithDigits(10);
}

std::string RandomDataGenerator::generateStringWithDigits(unsigned int length) {
    static std::uniform_int_distribution<> dist(0, 9);
    std::random_device dev;
    std::mt19937 generator(dev());

    std::ostringstream result;
    for (int i = 0; i < length; ++i)
        result << dist(generator);

    return result.str();
}

