#pragma once
#include <stdexcept>
#include <string>

#include "Serializable.h"

/**
 * Error related to an invalid amount - thrown e.g. when the amount format is wrong
 * or if the result is less than zero during subtraction.
 */
class InvalidAmountError : public std::logic_error {
public:
    /**
     * Constructor
     * @param msg Message allowing to evaluate the source of the error
     */
    explicit InvalidAmountError(const std::string& msg);
};

/**
 * Class representing every amount handled by the bank, stores
 * data in integer form - zloty and grosz parts of the amount
 */
class Amount : public Serializable {
public:
    /**
     * Constructor of amount from zloty and grosz parts
     * @param zloty Zloty part
     * @param grosz Grosz part (if >= 100 it will be converted
     *              to the appropriate amount of zlotys and groszs)
     */
    Amount(unsigned int zloty, unsigned int grosz);

    /**
     * Constructor of amount from floating point form, creates the amount
     * by rounding down the given number to two decimal places
     * @param fromFloat Floating point number representing the amount
     */
    Amount(float fromFloat);

    /**
     * Factory method for amount from a given string.
     * @param str String representing the amount in format "...(,..) (zl)"
     * @throws InvalidAmountError If the amount was given in an invalid
     *                            format
     */
    static Amount fromString(const std::string& str);

    /**
     * Factory method for amount from JSON format (string)
     * @param j JSON object representing the amount as a string
     * @throws InvalidAmountError From Amount::fromString method
     */
    static Amount fromJSON(const json& j);

    json toJSON() const override;

    /**
     * @returns Zloty part of the amount
     */
    unsigned int getZloty() const;

    /**
     * @returns Grosz part of the amount
     */
    unsigned int getGrosz() const;

    /**
     * @return Representation of the amount as a string - format "...,..zl"
     */
    std::string toString() const;

    /**
     * Multiplication of the amount by a floating point number (e.g. to calculate
     * a given percentage of the amount). The result may be inaccurate.
     * @param right Multiplier
     * @returns Multiplied amount
     */
    Amount operator*(float right) const;

    /**
     * Overloading the addition operator for amounts
     * @param right Amount on the right side of the "+" sign
     * @return Sum of the amount on which this method was called and the amount
     *         given as an argument
     */
    Amount operator+(const Amount& right) const;

    /**
     * Overloading the assignment operator with addition for amounts
     * @param right Amount on the right side of the "+=" operator
     */
    void operator+=(const Amount& right);

    /**
     * Overloading the assignment operator with subtraction for amounts
     * @param right Amount on the right side of the "-=" operator
     * @throws InvalidAmountError Throws an exception when the subtracted amount is
     *                            greater than the amount on which the operator was called.
     */
    void operator-=(const Amount& right);

    /**
     * Overloading the subtraction operator for amounts
     * @param right Amount on the right side of the "-" operator
     * @throws InvalidAmountError Throws an exception if the subtracted amount is
     *                            greater than the amount on which the operator was called.
     * @returns Difference of the amount on which this method was called and the amount
     *          given as an argument
     */
    Amount operator-(const Amount& right) const;

    /**
     * @param right Amount on the right side of the operator
     * @return whether amounts are equal
     */
    bool operator==(const Amount& right) const;

    /**
     * @param right Amount on the right side of the operator
     * @return whether amounts are different
     */
    bool operator!=(const Amount& right) const;

    /**
     * @param right Amount on the right side of the operator
     * @return whether the amount on the right is greater than the amount on the left
     */
    bool operator<(const Amount& right) const;

    /**
     * @param right Amount on the right side of the operator
     * @return whether the amount on the left is greater than the amount on the right
     */
    bool operator>(const Amount& right) const;

    /**
     * @param right Amount on the right side of the operator
     * @return whether the amount on the right is greater than or equal to the amount on the left
     */
    bool operator<=(const Amount& right) const;

    /**
     * @param right Amount on the right side of the operator
     * @return whether the amount on the left is greater than or equal to the amount on the right
     */
    bool operator>=(const Amount& right) const;
private:
    unsigned int zloty, grosz;
};

/**
 * Allows printing the amount to a stream
 * Required for proper compilation of boost tests
 * @param os Output stream
 * @param amount Amount being printed
 * @return Changed stream
 */
std::ostream& operator<<(std::ostream& os, const Amount& amount);
