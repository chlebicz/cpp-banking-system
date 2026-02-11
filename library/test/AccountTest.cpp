#include <boost/test/unit_test.hpp>
#include <memory>
#include "model/Account.h"
#include "model/InvestmentAccount.h"
#include "model/SavingsAccount.h"
#include "model/StandardCard.h"
#include <boost/date_time.hpp>

#include "model/Bank.h"
namespace gr = boost::gregorian;

BOOST_AUTO_TEST_SUITE(AccountTest)

class TestAccount : public Account {
public:
    TestAccount(std::string accountNumber, std::string ownerID)
        : Account(std::move(accountNumber), std::move(ownerID)) {}

    AccountType getType() const override {
        return AccountType::Main;
    }
};

/**
 * @test Test konstruktora konta
 */
BOOST_AUTO_TEST_CASE(AccountConstructorTest) {
    std::shared_ptr<Account> account = std::make_shared<TestAccount>("5", "55");
    BOOST_TEST(account->getAccountNumber() == "5");
    BOOST_TEST(account->getOwnerID() == "55");
}

/**
 * @test Test metody setBalance
 */
BOOST_AUTO_TEST_CASE(AccountSetBalanceTest) {
    std::shared_ptr<Account> account = std::make_shared<TestAccount>("5", "55");
    account->setBalance(100);
    BOOST_TEST(account->getBalance().getZloty() == 100);
    BOOST_TEST(account->getBalance().getGrosz() == 0);
}

/**
 * @test Test dodawania karty
 */
BOOST_AUTO_TEST_CASE(AccountAddCardTest) {
    std::shared_ptr<Account> account = std::make_shared<TestAccount>("5", "55");
    std::shared_ptr<Card> card = std::make_shared<StandardCard>();
    account->addCard(card);
    BOOST_TEST(account->getCards().size() == 1);
}

/**
 * @test Test usuwania karty
 */
BOOST_AUTO_TEST_CASE(AccountRemoveCardTest) {
    std::shared_ptr<Account> account = std::make_shared<TestAccount>("5", "55");
    std::shared_ptr<Card> card = std::make_shared<StandardCard>();
    account->addCard(card);
    BOOST_TEST_REQUIRE(account->getCards().size() == 1);
    account->removeCard(card);
    BOOST_TEST(account->getCards().size() == 0);
}

/**
 * @test Test transakcji za pomocą karty
 */
BOOST_AUTO_TEST_CASE(AccountTransactionTest) {
    std::shared_ptr<Account> senderAccount = std::make_shared<TestAccount>("5", "55");
    std::shared_ptr<Account> receiverAccount = std::make_shared<TestAccount>("7", "77");
    std::shared_ptr<Card> assignedCard = std::make_shared<StandardCard>();
    Bank::getInstance().setBalance(10000000);
    senderAccount->setBalance(200);
    BOOST_TEST_REQUIRE(senderAccount->transaction(100, receiverAccount) == false);
    senderAccount->addCard(assignedCard);
    BOOST_TEST(senderAccount->transaction(300, receiverAccount) == false);
    BOOST_TEST_REQUIRE(senderAccount->transaction(100, receiverAccount));
    BOOST_TEST(senderAccount->getBalance().getZloty() == 93);
    BOOST_TEST(receiverAccount->getBalance().getZloty() == 100);
    BOOST_TEST(Bank::getInstance().getBalance().getZloty() == 10000007);
}

/**
 * @test Test zakupu i sprzedaży złota za pomocą konta inwestycyjengo
 */
BOOST_AUTO_TEST_CASE(InvestmentAccountGoldTest) {
    std::shared_ptr<InvestmentAccount> account = std::make_shared<InvestmentAccount>("5", "55");
    account->setBalance(20000);
    BOOST_REQUIRE_THROW(account->buyGold(50), NotEnoughMoney);
    BOOST_REQUIRE_THROW(account->sellGold(), GoldError);

    account->buyGold(5);
    BOOST_TEST(account->getGoldCoins()->getCount() == 5);
    BOOST_TEST(account->getBalance().getZloty() == 15000);

    BOOST_TEST(account->sellGold() == 4900);
    BOOST_TEST(account->getBalance().getZloty() == 19900);
}

/**
 * @test Test zakładania lokaty na koncie oszczędnościowym
 */
BOOST_AUTO_TEST_CASE(SavingsAccountCreateDepositTest) {
    std::shared_ptr<SavingsAccount> account = std::make_shared<SavingsAccount>("5", "55");
    std::shared_ptr<SavingsAccount> account2 = std::make_shared<SavingsAccount>("7", "77");
    account->setBalance(20000.52);
    account2->setBalance(20000.52);
    BOOST_TEST(account->createDeposit(20001) == false);
    BOOST_TEST_REQUIRE(account->createDeposit(3000));
    BOOST_TEST_REQUIRE(account->getDeposit() != nullptr);
    BOOST_TEST(account->getDeposit()->getBeginTime() == gr::day_clock::local_day());
    BOOST_TEST(account->getDeposit()->getAmount().getZloty() == 3000);
    BOOST_TEST(account->getBalance().getZloty() == 17000);
    BOOST_TEST(account->createDeposit(5000) == false);
    gr::date date(2022, 5, 10);
    BOOST_TEST_REQUIRE(account2->createDeposit(3000, date));
    BOOST_TEST(account2->getDeposit()->getBeginTime() == date);
}

/**
 * @test Test usuwania lokaty z konta oszczędnościowego
 */
BOOST_AUTO_TEST_CASE(SavingsAccountDeleteDepositTest) {
    std::shared_ptr<SavingsAccount> account = std::make_shared<SavingsAccount>("5", "55");
    account->setBalance(20000.52);
    BOOST_TEST_REQUIRE(account->createDeposit(3000));
    BOOST_TEST_REQUIRE(account->getDeposit() != nullptr);
    account->deleteDeposit();
    BOOST_TEST(account->getDeposit() == nullptr);
}

BOOST_AUTO_TEST_SUITE_END()