#include <boost/test/unit_test.hpp>
#include <memory>
#include "model/Deposit.h"
#include <boost/date_time.hpp>

#include "model/Bank.h"
namespace gr = boost::gregorian;

BOOST_AUTO_TEST_SUITE(DepositTest)

/**
 * @test Sprawdza konstruktor lokaty
 */
BOOST_AUTO_TEST_CASE(DepositConstructorTest) {
    Amount bank = Bank::getInstance().getBalance();
    gr::date date = gr::day_clock::local_day() - gr::months(1);
    Bank::getInstance().setBalance(10000000);
    std::shared_ptr<Deposit> deposit = std::make_shared<Deposit>(100, date);
    BOOST_TEST(Bank::getInstance().getBalance().getZloty() == 10000100);
    BOOST_TEST_REQUIRE(deposit->getAmount().getZloty() == 100);
    BOOST_TEST_REQUIRE(deposit->getBeginTime() == date);
    BOOST_TEST(deposit->endDeposit().getZloty() == 102);
    BOOST_TEST(Bank::getInstance().getBalance().getZloty() == 9999998);
}

BOOST_AUTO_TEST_SUITE_END()