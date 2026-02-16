#include <boost/test/unit_test.hpp>

#include "model/Bank.h"

BOOST_AUTO_TEST_SUITE(AccountManagerTest)

/**
 * @test Checks client registration using the manager
 */
BOOST_AUTO_TEST_CASE(RegisterClientTest) {
    ClientManager clientManager;

    std::string name = "Hartosz", lastName = "Borna", login = "urzytkownik",
        password = "duma1234", personalID = "123456789";

    BOOST_REQUIRE(!clientManager.getClient(personalID));

    auto registered = clientManager.registerClient(
        name, lastName, personalID, login, password
    );

    auto client = clientManager.getClient(personalID);

    BOOST_TEST(client);
    BOOST_TEST(client == registered);
    BOOST_TEST(client->getName() == name);
    BOOST_TEST(client->getLastName() == lastName);
    BOOST_TEST(client->getLogin() == login);
    BOOST_TEST(client->getPassword() == password);
    BOOST_TEST(client->getPersonalId() == personalID);
}

/**
 * @test Checks if registerClient throws an exception if a client
 *       with the given PESEL number already exists
 */
BOOST_AUTO_TEST_CASE(RegisterClientThrowsGivenExistingIDTest) {
    ClientManager clientManager;

    std::string name = "Hartosz", lastName = "Borna", login = "urzytkownik",
        password = "duma1234", personalID = "123456789";
    clientManager.registerClient(name, lastName, personalID, login, password);

    BOOST_REQUIRE_THROW(
        clientManager.registerClient(
            "Chikolaj", "Mlebicz", personalID, "kownik", "1234duma"
        ),
        RegisterFailedError
    );
}

/**
 * @test Checks if registerClient throws exceptions if a client
 *       with the given login exists
 */
BOOST_AUTO_TEST_CASE(RegisterClientThrowsGivenExistingLoginTest) {
    ClientManager clientManager;

    std::string name = "Hartosz", lastName = "Borna", login = "urzytkownik",
        password = "duma1234", personalID = "123456789";
    clientManager.registerClient(name, lastName, personalID, login, password);

    BOOST_REQUIRE_THROW(
        clientManager.registerClient(
            "Chikolaj", "Mlebicz", "932184", login, "1234duma"
        ),
        RegisterFailedError
    );
}

/**
 * @test Checks if getClient returns nullptr if a client with the given
 *       PESEL number does not exist
 */
BOOST_AUTO_TEST_CASE(GetClientReturnsNullptrGivenNonexistentIDTest) {
    ClientManager clientManager;

    BOOST_TEST(clientManager.getClient("nonexistent") == nullptr);
}

/**
 * @test Checks correct login using the manager
 */
BOOST_AUTO_TEST_CASE(LoginTest) {
    ClientManager clientManager;

    std::string name = "Hartosz", lastName = "Borna", login = "urzytkownik",
        password = "duma1234", personalID = "123456789";
    auto registeredClient = clientManager.registerClient(
        name, lastName, personalID, login, password
    );

    auto loggedClient = clientManager.login(login, password);

    BOOST_TEST(loggedClient->getId() == registeredClient->getId());
}

/**
 * @test Checks if login throws an exception if a client
 *       with the given login does not exist
 */
BOOST_AUTO_TEST_CASE(LoginInvalidUsernameTest) {
    ClientManager clientManager;

    std::string name = "Hartosz", lastName = "Borna", login = "urzytkownik",
        password = "duma1234", personalID = "123456789";
    auto registeredClient = clientManager.registerClient(
        name, lastName, personalID, login, password
    );

    BOOST_REQUIRE_THROW(clientManager.login("abcd", password), LoginFailedError);
}

/**
 * @test Checks if login throws an exception if the entered password is
 *       incorrect
 */
BOOST_AUTO_TEST_CASE(LoginInvalidPasswordTest) {
    ClientManager clientManager;

    std::string name = "Hartosz", lastName = "Borna", login = "urzytkownik",
        password = "duma1234", personalID = "123456789";
    auto registeredClient = clientManager.registerClient(
        name, lastName, personalID, login, password
    );

    BOOST_REQUIRE_THROW(clientManager.login(login, "abcd1234"), LoginFailedError);
}

/**
 * @test Checks if the client's account is locked after 5 failed login attempts
 */
BOOST_AUTO_TEST_CASE(LoginInvalidPasswordAccountLockTest) {
    ClientManager clientManager;

    std::string name = "Hartosz", lastName = "Borna", login = "urzytkownik",
        password = "duma1234", personalID = "123456789";
    auto registeredClient = clientManager.registerClient(
        name, lastName, personalID, login, password
    );

    BOOST_REQUIRE_THROW(clientManager.login(login, "abcd1234"), LoginFailedError);
    BOOST_REQUIRE_THROW(clientManager.login(login, "abcd1234"), LoginFailedError);
    BOOST_REQUIRE_THROW(clientManager.login(login, "abcd1234"), LoginFailedError);
    BOOST_REQUIRE_THROW(clientManager.login(login, "abcd1234"), LoginFailedError);
    BOOST_REQUIRE_THROW(clientManager.login(login, "abcd1234"), LoginFailedError);

    BOOST_REQUIRE_THROW(clientManager.login(login, password), LoginFailedError);
}

/**
 * @test Checks if after a successful login the counter of failed login attempts
 *       before lock is reset
 */
BOOST_AUTO_TEST_CASE(IncorrectLoginsCountResetTest) {
    ClientManager clientManager;

    std::string name = "Hartosz", lastName = "Borna", login = "urzytkownik",
        password = "duma1234", personalID = "123456789";
    auto registeredClient = clientManager.registerClient(
        name, lastName, personalID, login, password
    );

    BOOST_REQUIRE_THROW(clientManager.login(login, "abcd1234"), LoginFailedError);
    BOOST_REQUIRE_THROW(clientManager.login(login, "abcd1234"), LoginFailedError);
    BOOST_REQUIRE_THROW(clientManager.login(login, "abcd1234"), LoginFailedError);

    auto client = clientManager.login(login, password);
    BOOST_TEST(client->getIncorrectLogins() == 0);
}

/**
 * @test Checks finding a client by predicate
 */
BOOST_AUTO_TEST_CASE(FindClientTest) {
    ClientManager clientManager;

    std::string name = "Hartosz", lastName = "Borna", login = "urzytkownik",
        password = "duma1234", personalID = "123456789";
    auto client = clientManager.registerClient(
        name, lastName, personalID, login, password
    );

    auto result = clientManager.findClient([&login](const auto& c) {
        return c->getLogin() == login;
    });
    BOOST_TEST(result == client);
}

/**
 * @test Negative test of finding a client by predicate
 */
BOOST_AUTO_TEST_CASE(FindClientNegativeTest) {
    ClientManager clientManager;

    std::string name = "Hartosz", lastName = "Borna", login = "urzytkownik",
        password = "duma1234", personalID = "123456789";
    auto client = clientManager.registerClient(
        name, lastName, personalID, login, password
    );

    auto result = clientManager.findClient([&login](const auto& c) {
        return c->getLogin() == "a";
    });
    BOOST_TEST(result == nullptr);
}

/**
 * @test Test finding all clients satisfying the given predicate
 */
BOOST_AUTO_TEST_CASE(FindAllClientsTest) {
    ClientManager clientManager;

    auto client = clientManager.registerClient("a", "b", "c", "d", "e");
    auto client2 = clientManager.registerClient("f", "g", "h", "i", "j");

    auto result = clientManager.findAllClients([] (const auto&) {
        return true;
    });
    BOOST_TEST(result.size() == 2);
}

/**
 * @test Negative test of finding all clients satisfying the given predicate
 */
BOOST_AUTO_TEST_CASE(FindAllClientsNegativeTest) {
    ClientManager clientManager;

    auto client = clientManager.registerClient("a", "b", "c", "d", "e");
    auto client2 = clientManager.registerClient("f", "g", "h", "i", "j");

    auto result = clientManager.findAllClients([] (const auto&) {
        return false;
    });
    BOOST_TEST(result.empty());
}

/**
 * @test Client removal test
 */
BOOST_AUTO_TEST_CASE(UnregisterClientTest) {
    ClientManager clientManager;

    std::string name = "Hartosz", lastName = "Borna", login = "urzytkownik",
        password = "duma1234", personalID = "123456789";
    clientManager.registerClient(name, lastName, personalID, login, password);

    BOOST_TEST(clientManager.unregisterClient(personalID));
    BOOST_TEST(clientManager.getClient(personalID) == nullptr);
    BOOST_TEST(clientManager.getAllClients().empty());
}

/**
 * @test Checks if unregisterClient does nothing if a client with the given
 *       PESEL number does not exist
 */
BOOST_AUTO_TEST_CASE(UnregisterClientDoesNothingGivenNonexistentIDTest) {
    ClientManager clientManager;

    std::string name = "Hartosz", lastName = "Borna", login = "urzytkownik",
        password = "duma1234", personalID = "123456789";
    auto registered = clientManager.registerClient(
        name, lastName, personalID, login, password
    );

    BOOST_TEST(!clientManager.unregisterClient("321321"));
    BOOST_TEST(clientManager.getClient(personalID) == registered);
    BOOST_TEST(clientManager.getAllClients().size() == 1);
}

/**
 * @test Checks save and then read of saved objects from disk
 */
BOOST_AUTO_TEST_CASE(ClientManSaveLoadTest) {
    std::string name = "Hartosz", lastName = "Borna", login = "urzytkownik",
        password = "duma1234", personalID = "123456789";

    {
        ClientManager clientManager;

        clientManager.registerClient(
            name, lastName, personalID, login, password
        );
        clientManager.save();
    }

    ClientManager clientManager;
    clientManager.load();

    BOOST_TEST(clientManager.getAllClients().size() == 1);
    BOOST_TEST(clientManager.getClient(personalID));
}

/**
 * @test Checks if after deleting an object and saving, the object does not appear
 *       on read
 */
BOOST_AUTO_TEST_CASE(ClientManDeleteLoadTest) {
    std::string name = "Hartosz", lastName = "Borna", login = "urzytkownik",
        password = "duma1234", personalID = "123456789";

    {
        ClientManager clientManager;

        clientManager.registerClient(
            name, lastName, personalID, login, password
        );
        clientManager.save();
    }

    {
        ClientManager clientManager;
        clientManager.load();

        clientManager.unregisterClient(personalID);
        clientManager.save();
    }

    ClientManager clientManager;
    clientManager.load();
    BOOST_TEST(clientManager.getAllClients().empty());
}

BOOST_AUTO_TEST_SUITE_END()
