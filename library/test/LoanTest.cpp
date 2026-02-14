#include <boost/test/unit_test.hpp>
#include <memory>
#include "model/Loan.h"
#include "model/Account.h"
#include <boost/date_time.hpp>

#include "model/Bank.h"
#include "model/MainAccount.h"
namespace gr = boost::gregorian;

BOOST_AUTO_TEST_SUITE(LoanTest)

/**
 * @test Checks correctness of loan constructor
 */
BOOST_AUTO_TEST_CASE(LoanConstructorTest) {
    std::shared_ptr<Account> account = std::make_shared<MainAccount>("5", "55");
    account->setBalance(200);
    Bank::getInstance().setBalance(10000000);
    std::shared_ptr<Loan> loan = std::make_shared<Loan>(
        24, 100000, gr::day_clock::local_day(), account
    );
    loan->create();
    BOOST_TEST(Bank::getInstance().getBalance().getZloty() == 9900000);
    BOOST_TEST_REQUIRE(loan->getMonths() == 24);
    BOOST_TEST_REQUIRE(loan->getAmount() == 100000);
    BOOST_TEST(loan->getBeginTime() == gr::day_clock::local_day());
    BOOST_TEST_REQUIRE(loan->getOperationalAccount() == account);
    BOOST_TEST(loan->getMontlyPayment() == 5000);
    BOOST_TEST(account->getBalance() == 100200);
}

/**
 * @test Checks correct calculation of remaining loan months
 */
BOOST_AUTO_TEST_CASE(LoanRemainingMonthsTest) {
    std::shared_ptr<Account> account = std::make_shared<MainAccount>("5", "55");
    gr::date date = gr::day_clock::local_day() - gr::months(1);
    std::shared_ptr<Loan> loan = std::make_shared<Loan>(
        24, 100000, date, account
    );
    loan->create();
    BOOST_TEST(loan->getRemainingMonths() == 23);
}

/**
 * @test Checks correct taking of loan installments
 */
BOOST_AUTO_TEST_CASE(LoanTakeMoneyTest) {
    std::shared_ptr<Account> account = std::make_shared<MainAccount>("5", "55");
    gr::date date = gr::day_clock::local_day() - gr::months(1);
    std::shared_ptr<Loan> loan = std::make_shared<Loan>(
        24, 100000, date, account
    );
    loan->create();
    Bank::getInstance().setBalance(1000000);
    loan->takeMoney();
    BOOST_TEST(account->getBalance() == 95000);
    BOOST_TEST(Bank::getInstance().getBalance().getZloty() == 1005000);
}

BOOST_AUTO_TEST_SUITE_END()
