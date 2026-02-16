#include <boost/test/unit_test.hpp>

#include "managers/AccountManager.h"
#include "managers/ClientManager.h"

std::ostream& operator<<(std::ostream& os, AccountType type) {
    os << static_cast<int>(type);
    return os;
}

BOOST_AUTO_TEST_SUITE(AccountManagerTest)

/**
 * @test Opening an account using account manager
 */
BOOST_AUTO_TEST_CASE(OpenAccountTest) {
    AccountManager accountManager;

    const std::string ownerID = "123";
    const auto accountType = AccountType::Main;
    auto newAccount = accountManager.openAccount(ownerID, accountType);

    BOOST_TEST(newAccount->getOwnerID() == ownerID);
    BOOST_TEST(newAccount->getType() == accountType);

    auto account = accountManager.getAccount(newAccount->getId());

    BOOST_TEST(account == newAccount);
}

/**
 * @test Closing an account
 */
BOOST_AUTO_TEST_CASE(CloseAccountTest) {
    AccountManager accountManager;

    auto newAccount = accountManager.openAccount("123", AccountType::Main);
    accountManager.closeAccount(newAccount->getId());

    BOOST_TEST(accountManager.getAllAccounts().empty());
}

/**
 * @test Finding accounts belonging to a client
 */
BOOST_AUTO_TEST_CASE(FindClientAccountsTest) {
    AccountManager accountManager;

    auto account1 = accountManager.openAccount("123", AccountType::Main);
    auto account2 = accountManager.openAccount("123", AccountType::Investment);

    auto account3 = accountManager.openAccount("321", AccountType::Main);

    BOOST_TEST(accountManager.findClientAccounts("123").size() == 2);
    BOOST_TEST(accountManager.findClientAccounts("321").size() == 1);
}

/**
 * @test Save and then load saved account objects from disk
 */
BOOST_AUTO_TEST_CASE(AccountManagerSaveLoadTest) {
    const std::string ownerID = "123";
    const auto type = AccountType::Main;

    {
        AccountManager accountManager;

        accountManager.openAccount(ownerID, type);
        accountManager.save();
    }

    AccountManager accountManager;
    accountManager.load();

    BOOST_TEST(accountManager.getAllAccounts().size() == 1);
    BOOST_TEST(accountManager.findClientAccounts(ownerID).size() == 1);
    BOOST_TEST(accountManager.findClientAccounts(ownerID)[0]->getType() == type);
}

/**
 * @test Checks if after deleting an account and loading from disk the account is deleted
 */
BOOST_AUTO_TEST_CASE(AccountManagerDeleteLoadTest) {
    std::string id;

    {
        AccountManager accountManager;
        auto newAccount = accountManager.openAccount(
            "123", AccountType::Main
        );
        id = newAccount->getId();
        accountManager.save();
    }

    {
        AccountManager accountManager;
        accountManager.load();
        accountManager.closeAccount(id);
        accountManager.save();
    }

    AccountManager accountManager;
    accountManager.load();
    BOOST_TEST(accountManager.getAllAccounts().empty());
}

BOOST_AUTO_TEST_SUITE_END()
