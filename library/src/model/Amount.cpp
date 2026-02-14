#include "model/Amount.h"
#include <regex>

InvalidAmountError::InvalidAmountError(const std::string& msg)
    : std::logic_error(msg) {}

Amount::Amount(unsigned int zloty, unsigned int grosz)
    : zloty(zloty + grosz / 100), grosz(grosz % 100)
{}

Amount::Amount(float fromFloat)
    : zloty(static_cast<unsigned int>(fromFloat)),
      grosz(static_cast<unsigned int>(fromFloat * 100) - zloty * 100)
{}

Amount Amount::fromString(const std::string& str) {
    static const std::regex pattern("^(\\d+)(,(\\d{2}))?(\\s*zl)?$");

    std::smatch match;
    if (!std::regex_match(str, match, pattern))
        throw InvalidAmountError{"Invalid amount format"};

    unsigned int zloty = std::stoi(match[1].str());
    unsigned int grosz = 0;
    if (match[3].matched)
        grosz = std::stoi(match[3].str());

    return Amount(zloty, grosz);
}

Amount Amount::fromJSON(const json& j) {
    return fromString(j);
}

json Amount::toJSON() const {
    return toString();
}

unsigned int Amount::getZloty() const {
    return zloty;
}

unsigned int Amount::getGrosz() const {
    return grosz;
}

std::string Amount::toString() const {
    auto groszStr = std::to_string(grosz);
    if (groszStr.length() == 1)
        groszStr = "0" + groszStr;

    return std::to_string(zloty) + "," + groszStr + " zl";
}

Amount Amount::operator*(float right) const {
    float value = static_cast<float>(zloty) + static_cast<float>(grosz) / 100;
    return {value * right};
}

Amount Amount::operator+(const Amount& right) const {
    return {
        zloty + right.zloty,
        grosz + right.grosz
    };
}

void Amount::operator+=(const Amount& right) {
    Amount result = *this + right;
    zloty = result.zloty;
    grosz = result.grosz;
}

void Amount::operator-=(const Amount& right) {
    Amount result = *this - right;
    zloty = result.zloty;
    grosz = result.grosz;
}

Amount Amount::operator-(const Amount& right) const {
    if (right > *this)
        throw InvalidAmountError{"Result of amount subtraction less than zero"};

    unsigned int zloty = this->zloty, grosz = this->grosz;
    if (right.grosz > grosz) {
        zloty -= 1;
        grosz += 100;
    }

    return {zloty - right.zloty, grosz - right.grosz};
}

bool Amount::operator==(const Amount& right) const {
    return zloty == right.zloty && grosz == right.grosz;
}

bool Amount::operator!=(const Amount& right) const {
    return !operator==(right);
}

bool Amount::operator<(const Amount& right) const {
    return zloty < right.zloty || (zloty == right.zloty && grosz < right.grosz);
}

bool Amount::operator>(const Amount& right) const {
    return zloty > right.zloty || (zloty == right.zloty && grosz > right.grosz);
}

bool Amount::operator<=(const Amount& right) const {
    return !(*this > right);
}

bool Amount::operator>=(const Amount& right) const {
    return !(*this < right);
}

std::ostream& operator<<(std::ostream &os, const Amount& amount) {
    os << amount.toString();
    return os;
}
