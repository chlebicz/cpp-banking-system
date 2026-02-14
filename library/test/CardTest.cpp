#include <boost/test/unit_test.hpp>
#include <memory>
#include "model/StandardCard.h"
#include "model/GoldCard.h"
#include "model/DiamondCard.h"

BOOST_AUTO_TEST_SUITE(CardTest)

/**
 * @test Checks Standard card constructor
 */
BOOST_AUTO_TEST_CASE(StandardCardTest) {
    std::shared_ptr<Card> card = std::make_shared<StandardCard>();
    Amount amount(7);
    BOOST_TEST(card->calculateFee(100).getZloty() == amount.getZloty());
}

/**
 * @test Checks Gold card constructor
 */
BOOST_AUTO_TEST_CASE(GoldCardTest) {
    std::shared_ptr<Card> card = std::make_shared<GoldCard>();
    Amount amount(5);
    BOOST_TEST(card->calculateFee(100).getZloty() == amount.getZloty());
}

/**
 * @test Checks Diamond card constructor
 */
BOOST_AUTO_TEST_CASE(DiamondCardTest) {
    std::shared_ptr<Card> card = std::make_shared<DiamondCard>();
    Amount amount(2);
    BOOST_TEST(card->calculateFee(100).getZloty() == amount.getZloty());
}

BOOST_AUTO_TEST_SUITE_END()
