#include <boost/test/unit_test.hpp>
#include <memory>
#include "model/Client.h"
#include <boost/date_time.hpp>

#include "model/MainAccount.h"
namespace gr = boost::gregorian;

BOOST_AUTO_TEST_SUITE(ClientTest)

/**
 * @test Checks correctness of client constructor (e.g. parameter order)
 */
BOOST_AUTO_TEST_CASE(ClientConstructorTest) {
    std::shared_ptr<Client> client = std::make_shared<Client>("John", "Doe", "5", "123", "123");
    BOOST_TEST(client->getName() == "John");
    BOOST_TEST(client->getLastName() == "Doe");
    BOOST_TEST(client->getPersonalId() == "5");
    BOOST_TEST(client->getLogin() == "123");
    BOOST_TEST(client->getPassword() == "123");
}

/**
 * @test Checks functionality of setters in client
 */
BOOST_AUTO_TEST_CASE(ClientSetterTest) {
    std::shared_ptr<Client> client = std::make_shared<Client>("John", "Doe", "5", "123", "123");
    client->setName("Jane");
    client->setLastName("Smith");
    client->setLogin("321");
    client->setPassword("321");
    BOOST_TEST(client->getName() == "Jane");
    BOOST_TEST(client->getLastName() == "Smith");
    BOOST_TEST(client->getLogin() == "321");
    BOOST_TEST(client->getPassword() == "321");
}

/**
 * @test Checks taking a loan
 */
BOOST_AUTO_TEST_CASE(ClientLoansTest) {
    std::shared_ptr<Client> client = std::make_shared<Client>("John", "Doe", "5", "123", "123");
    std::shared_ptr<Account> account = std::make_shared<MainAccount>("5", "5");
    gr::date date1 = gr::day_clock::local_day() - gr::months(1);
    gr::date date2 = gr::day_clock::local_day() - gr::months(24);
    client->takeLoan(24, 100000, account, date1);
    client->takeLoan(24, 100000, account, date2);
    BOOST_TEST_REQUIRE(account->getBalance().getZloty() == 200000);
    BOOST_TEST_REQUIRE(client->getLoans().size() == 2);
    client->payForLoans();
    BOOST_TEST(account->getBalance().getZloty() == 75000);
    BOOST_TEST(client->getLoans().size() == 1);
}

BOOST_AUTO_TEST_SUITE_END()
