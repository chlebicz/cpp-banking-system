#include <boost/test/unit_test.hpp>

#include "model/Amount.h"

BOOST_AUTO_TEST_SUITE(AmountTest)

/**
 * @test Test constructor from two ints
 */
BOOST_AUTO_TEST_CASE(AmountIntConstructorTest) {
    Amount amount{100, 63};
    BOOST_TEST(amount.getZloty() == 100);
    BOOST_TEST(amount.getGrosz() == 63);
}

/**
 * @test Test constructor from two ints, where the given grosz part
 *       is greater than 99
 */
BOOST_AUTO_TEST_CASE(AmountIntConstructorOverflowTest) {
    Amount amount{100, 199};
    BOOST_TEST(amount.getZloty() == 101);
    BOOST_TEST(amount.getGrosz() == 99);
}

/**
 * @test Test amount constructor from a float number
 */
BOOST_AUTO_TEST_CASE(AmountFloatConstructorTest) {
    Amount amount{100.637};
    BOOST_TEST(amount.getZloty() == 100);
    BOOST_TEST(amount.getGrosz() == 63);
}

/**
 * @test Test creating amount from string
 */
BOOST_AUTO_TEST_CASE(AmountFromStringTest) {
    auto amount1 = Amount::fromString("36");
    BOOST_TEST(amount1.getZloty() == 36);
    BOOST_TEST(amount1.getGrosz() == 00);

    auto amount2 = Amount::fromString("36,41   zl");
    BOOST_TEST(amount2.getZloty() == 36);
    BOOST_TEST(amount2.getGrosz() == 41);

    auto amount3 = Amount::fromString("21,37");
    BOOST_TEST(amount3.getZloty() == 21);
    BOOST_TEST(amount3.getGrosz() == 37);
}

/**
 * @test Negative test (throwing exceptions) of creating amount from string
 */
BOOST_AUTO_TEST_CASE(AmountFromStringExceptionTest) {
    BOOST_REQUIRE_THROW(Amount::fromString("100,999"), InvalidAmountError);
    BOOST_REQUIRE_THROW(Amount::fromString("6,90 zuotych"), InvalidAmountError);
    BOOST_REQUIRE_THROW(Amount::fromString("4.20 zl"), InvalidAmountError);
    BOOST_REQUIRE_THROW(Amount::fromString("4,2 zl"), InvalidAmountError);
}

/**
 * @test toString method test
 */
BOOST_AUTO_TEST_CASE(AmountToStringTest) {
    Amount amount{2137, 42};
    BOOST_TEST(amount.toString() == "2137,42 zl");
}

/**
 * @test Checks if zeros are added at the beginning in string representation
 *       if the grosz part is less than 10
 */
BOOST_AUTO_TEST_CASE(AmountToStringAddsLeadingZeroTest) {
    Amount amount{2137, 2};
    BOOST_TEST(amount.toString() == "2137,02 zl");
}

/**
 * @test Checks adding two amounts using operator overloading
 *       In the checked case, the sum of grosz parts is greater than 99
 */
BOOST_AUTO_TEST_CASE(AmountOperatorPlusTest) {
    Amount amount1{210, 73};
    Amount amount2{46, 43};

    Amount sum = amount1 + amount2;

    BOOST_TEST(sum.getZloty() == 257);
    BOOST_TEST(sum.getGrosz() == 16);
}

/**
 * @test Checks multiplying amount by a float number
 */
BOOST_AUTO_TEST_CASE(AmountOperatorTimesFloatTest) {
    Amount amount{100,00};
    Amount product = amount * 0.02;
    BOOST_TEST(product.getZloty() == 2);
}

/**
 * @test Checks += operator for amount
 */
BOOST_AUTO_TEST_CASE(AmountOperatorPlusEqualsTest) {
    Amount amount1{210, 73};
    Amount amount2{46, 43};

    amount1 += amount2;

    BOOST_TEST(amount1.getZloty() == 257);
    BOOST_TEST(amount1.getGrosz() == 16);
}

/**
 * @test Checks -= operator for amount, where the grosz part of the subtracted
 *       amount is greater than the grosz part of the amount being subtracted from
 */
BOOST_AUTO_TEST_CASE(AmountOperatorMinusGreaterRightGroszTest) {
    Amount amount1{210, 43};
    Amount amount2{46, 73};

    Amount diff = amount1 - amount2;

    BOOST_TEST(diff.getZloty() == 163);
    BOOST_TEST(diff.getGrosz() == 70);
}

/**
 * @test Checks - operator for amount, where the grosz part of the subtracted
 *       amount is less than the grosz part of the amount being subtracted from
 */
BOOST_AUTO_TEST_CASE(AmountOperatorMinusLessRightGroszTest) {
    Amount amount1{210, 73};
    Amount amount2{46, 43};

    Amount diff = amount1 - amount2;

    BOOST_TEST(diff.getZloty() == 164);
    BOOST_TEST(diff.getGrosz() == 30);
}

/**
 * @test Checks if subtraction operator throws an exception if the subtracted
 *       amount is greater than the amount being subtracted from
 */
BOOST_AUTO_TEST_CASE(AmountOperatorMinusThrowsGivenRightGreaterTest) {
    Amount amount1{21, 37};
    Amount amount2{69, 42};

    BOOST_REQUIRE_THROW(amount1 - amount2, InvalidAmountError);
}

/**
 * @test Checks -= operator for amount
 */
BOOST_AUTO_TEST_CASE(AmountOperatorMinusEqualsTest) {
    Amount amount1{210, 43};
    Amount amount2{46, 73};

    amount1 -= amount2;

    BOOST_TEST(amount1.getZloty() == 163);
    BOOST_TEST(amount1.getGrosz() == 70);
}

/**
 * @test Checks if == operator returns true if amounts are equal
 */
BOOST_AUTO_TEST_CASE(AmountOperatorEqualsReturnsTrueGivenEqualTest) {
    Amount amount1{210, 43};
    Amount amount2{210, 43};

    BOOST_TEST(amount1 == amount2);
}

/**
 * @test Checks if == operator returns false if amounts are different
 */
BOOST_AUTO_TEST_CASE(AmountOperatorEqualsReturnsFalseGivenDifferentTest) {
    Amount amount1{210, 43};
    Amount amount2{46, 73};

    BOOST_TEST(!(amount1 == amount2));
}

/**
 * @test Checks if != operator returns true if amounts are different
 */
BOOST_AUTO_TEST_CASE(AmountOperatorNotEqualsReturnsTrueGivenDifferentTest) {
    Amount amount1{210, 43};
    Amount amount2{46, 73};

    BOOST_TEST(amount1 != amount2);
}

/**
 * @test Checks if != operator returns false if amounts are equal
 */
BOOST_AUTO_TEST_CASE(AmountOperatorNotEqualsReturnsFalseGivenEqualTest) {
    Amount amount1{210, 43};
    Amount amount2{210, 43};

    BOOST_TEST(!(amount1 != amount2));
}

/**
 * @test Checks if < operator returns true if amount on the right is less
 */
BOOST_AUTO_TEST_CASE(AmountOperatorLessReturnsTrueGivenLessTest) {
    Amount amount1{100, 10};
    Amount amount2{210, 43};

    BOOST_TEST(amount1 < amount2);
}

/**
 * @test Checks if < operator returns true if amount on the right is less,
 *       while zloty parts of both amounts are equal, differing only by
 *       grosz parts
 */
BOOST_AUTO_TEST_CASE(AmountOperatorLessReturnsTrueGivenLessGroszTest) {
    Amount amount1{210, 38};
    Amount amount2{210, 43};

    BOOST_TEST(amount1 < amount2);
}

/**
 * @test Checks if < operator returns false if amounts are equal
 */
BOOST_AUTO_TEST_CASE(AmountOperatorLessReturnsFalseGivenEqualTest) {
    Amount amount1{210, 43};
    Amount amount2{210, 43};

    BOOST_TEST(!(amount1 < amount2));
}

/**
 * @test Checks if < operator returns false if amount on the right is greater
 */
BOOST_AUTO_TEST_CASE(AmountOperatorLessReturnsFalseGivenGreaterTest) {
    Amount amount1{250, 78};
    Amount amount2{210, 43};

    BOOST_TEST(!(amount1 < amount2));
}

/**
 * @test Checks if > operator returns true if amount on the left is greater
 */
BOOST_AUTO_TEST_CASE(AmountOperatorGreaterReturnsTrueGivenGreaterTest) {
    Amount amount1{760, 53};
    Amount amount2{210, 43};

    BOOST_TEST(amount1 > amount2);
}

/**
 * @test Checks if > operator returns true if amount on the right is greater,
 *       while amounts do not differ by zloty part
 */
BOOST_AUTO_TEST_CASE(AmountOperatorGreaterReturnsTrueGivenGreaterGroszTest) {
    Amount amount1{210, 53};
    Amount amount2{210, 48};

    BOOST_TEST(amount1 > amount2);
}

/**
 * @test Checks if > operator returns true if amounts are equal
 */
BOOST_AUTO_TEST_CASE(AmountOperatorGreaterReturnsFalseGivenEqualTest) {
    Amount amount1{210, 43};
    Amount amount2{210, 43};

    BOOST_TEST(!(amount1 > amount2));
}

/**
 * @test Checks if > operator returns true if amount on the right is greater
 */
BOOST_AUTO_TEST_CASE(AmountOperatorGreaterReturnsFalseGivenLessTest) {
    Amount amount1{178, 78};
    Amount amount2{210, 43};

    BOOST_TEST(!(amount1 > amount2));
}

/**
 * @test Checks if >= operator returns true if amount on the left is greater
 */
BOOST_AUTO_TEST_CASE(AmountOperatorGreaterEqualReturnsTrueGivenGreaterTest) {
    Amount amount1{760, 53};
    Amount amount2{210, 43};

    BOOST_TEST(amount1 >= amount2);
}

/**
 * @test Checks if >= operator returns true if amounts are equal
 */
BOOST_AUTO_TEST_CASE(AmountOperatorGreaterEqualReturnsTrueGivenEqualTest) {
    Amount amount1{210, 43};
    Amount amount2{210, 43};

    BOOST_TEST(amount1 >= amount2);
}

/**
 * @test Checks if >= operator returns false if amount on the right is greater
 */
BOOST_AUTO_TEST_CASE(AmountOperatorGreaterEqualReturnsFalseGivenLessTest) {
    Amount amount1{210, 10};
    Amount amount2{210, 43};

    BOOST_TEST(!(amount1 >= amount2));
}

/**
 * @test Checks if <= operator returns true if amount on the left is less
 */
BOOST_AUTO_TEST_CASE(AmountOperatorLessEqualReturnsTrueGivenLessTest) {
    Amount amount1{200, 43};
    Amount amount2{210, 13};

    BOOST_TEST(amount1 <= amount2);
}

/**
 * @test Checks if <= operator returns true if amount on the right is greater
 */
BOOST_AUTO_TEST_CASE(AmountOperatorLessEqualReturnsTrueGivenEqualTest) {
    Amount amount1{210, 43};
    Amount amount2{210, 43};

    BOOST_TEST(amount1 <= amount2);
}

/**
 * @test Checks if <= operator returns false if amount on the left is greater
 */
BOOST_AUTO_TEST_CASE(AmountOperatorLessEqualReturnsFalseGivenGreaterTest) {
    Amount amount1{216, 43};
    Amount amount2{210, 43};

    BOOST_TEST(!(amount1 <= amount2));
}

BOOST_AUTO_TEST_SUITE_END()
