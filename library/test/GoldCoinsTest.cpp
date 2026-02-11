#include <boost/test/unit_test.hpp>

#include "model/GoldCoins.h"

BOOST_AUTO_TEST_SUITE(GoldCoinsTest)

/**
 * @test Sprawdza poprawność działania konstruktora obiektu złotych monet
 */
BOOST_AUTO_TEST_CASE(GoldCoinsConstructorTest) {
    int count = 5;
    auto purchaseTime = pt::ptime(
        boost::gregorian::date(2025, 6, 10)
    );
    GoldCoins coins{count, purchaseTime};

    BOOST_TEST(coins.getCount() == count);
    BOOST_TEST(coins.getPurchaseTime() == purchaseTime);
}

/**
 * @test Sprawdza, czy obliczona wartość zaraz po zakupie złotych monet
 *       nie zmieniła się i jest równa 1000 za każdą 1 monete
 */
BOOST_AUTO_TEST_CASE(GoldCoinsCalculateImmediateValueTest) {
    GoldCoins coins{5};
    BOOST_TEST(coins.calculateValue() == 5000);
}

/**
 * @test Sprawdza, czy po upłynięciu paru dni od zakupu złotych monet,
 *       cena rosła o 1 zł każdego dnia
 */
BOOST_AUTO_TEST_CASE(GoldCoinsCalculateValueTest) {
    int count = 5;
    auto purchaseTime = pt::ptime(
        boost::gregorian::date(2025, 6, 10)
    );
    GoldCoins coins{count, purchaseTime};

    auto sellTime = pt::ptime(
        boost::gregorian::date(2025, 6, 15)
    );
    BOOST_TEST(coins.calculateValue(sellTime) == 5025);
}

/**
 * @test Sprawdza poprawność ciągu znaków generowanego przez metodę toString
 */
BOOST_AUTO_TEST_CASE(GoldCoinsToStringTest) {
    int count = 5;
    auto purchaseTime = pt::ptime(
        boost::gregorian::date(2025, 6, 10)
    );
    GoldCoins coins{count, purchaseTime};

    BOOST_TEST(coins.toString() == "5 zlotych monet, zakupione 10.06.2025");
}

/**
 * @test Sprawdza, czy z obiektu JSON wygenerowanego przez funkcję toJSON
 *       można odczytać obiekt złotych monet za pomocą metody fromJSON
 */
BOOST_AUTO_TEST_CASE(GoldCoinsJSONTest) {
    int count = 5;
    auto purchaseTime = pt::ptime(
        boost::gregorian::date(2025, 6, 10)
    );
    GoldCoins coins{count, purchaseTime};

    auto json = coins.toJSON();
    auto fromJSON = GoldCoins::fromJSON(json);

    BOOST_TEST(fromJSON->getCount() == count);
    BOOST_TEST(fromJSON->getPurchaseTime() == purchaseTime);
}

BOOST_AUTO_TEST_SUITE_END()