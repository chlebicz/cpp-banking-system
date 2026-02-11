#include <boost/test/unit_test.hpp>

#include "model/Bank.h"

BOOST_AUTO_TEST_SUITE(AccountManagerTest)

/**
 * @test Sprawdza rejestrowanie klientów za pomocą menadżera
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
 * @test Sprawdza, czy registerClient wyrzuca wyjątek, jeśli klient
 *       o podanym numerze PESEL już istnieje
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
 * @test Sprawdza, czy registerClient wyrzuca wyjątki, jeśli klient
 *       o podanym loginie istnieje
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
 * @test Sprawdza, czy getClient zwraca nullptr, jeśli klient o podanym
 *       numerze PESEL już istnieje
 */
BOOST_AUTO_TEST_CASE(GetClientReturnsNullptrGivenNonexistentIDTest) {
    ClientManager clientManager;

    BOOST_TEST(clientManager.getClient("nieistniejacy") == nullptr);
}

/**
 * @test Sprawdza poprawne logowanie za pomocą menadżera
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
 * @test Sprawdza, czy login zgłasza wyjątek jeśli nie istnieje klient
 *       o podanym loginie
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
 * @test Sprawdza, czy login zgłasza wyjątek, jeśli wprowadzone hasło jest
 *       niepoprawne
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
 * @test Sprawdza, czy konto klienta jest blokowane po 5 nieudanych próbach
 *       logowania
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
 * @test Sprawdza, czy po poprawnym zalogowaniu licznik niepoprawnych prób
 *       zalogowania do blokady jest resetowany
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
 * @test Sprawdza wyszukiwanie klienta predykatem
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
 * @test Test negatywny wyszukiwania klienta predykatem
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
 * @test Test wyszukiwania wszystkich klientów spełniających podany predykat
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
 * @test Test negatywny wyszukiwania wszystkich klientów spełniających podany predykat
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
 * @test Test usunięcia klienta
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
 * @test Sprawdza, czy unregisterClient nie robi nic jeśli klient o podanym
 *       numerze PESEL nie istnieje
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
 * @test Sprawdza zapis, a następnie odczyt zapisanych obiektów z dysku
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
 * @test Sprawdza, czy po usunięciu obiektu i zapisie, obiekt nie pojawia
 *       się przy odczycie
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
