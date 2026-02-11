#include "managers/ClientManager.h"

LoginFailedError::LoginFailedError(const std::string& msg)
    : std::runtime_error(msg) {}

RegisterFailedError::RegisterFailedError(const std::string &msg)
    : std::runtime_error(msg) {}

std::shared_ptr<Client> ClientManager::registerClient(
    std::string firstName, std::string lastName, std::string personalID,
    std::string login, std::string password
) {
    auto client = getClient(personalID);
    if (client)
        throw RegisterFailedError{
            "Klient o podanym numerze PESEL jest juz zarejestrowany"
        };

    auto sameLogin = findClient([&login] (const auto& c) {
        return c->getLogin() == login;
    });
    if (sameLogin)
        throw RegisterFailedError{"Klient o podanym loginie juz istnieje"};

    client = std::make_shared<Client>(
        std::move(firstName), std::move(lastName), std::move(personalID),
        std::move(login), std::move(password)
    );
    repository.add(client);

    return client;
}

std::shared_ptr<Client> ClientManager::getClient(const std::string& personalID) {
    return repository.findFirst(
        [&personalID](const std::shared_ptr<Client>& client) {
            return client->getPersonalId() == personalID;
        }
    );
}

std::shared_ptr<Client> ClientManager::login(
    const std::string& login, const std::string& password
) {
    auto client = repository.findFirst(
        [&login](const std::shared_ptr<Client>& client) {
            return client->getLogin() == login;
        }
    );

    if (!client)
        throw LoginFailedError("Niepoprawna nazwa uzytkownika");

    if (client->isAccountLocked())
        throw LoginFailedError(
            "Twoje konto jest zablokowane. Nie udalo sie zalogowac"
        );

    if (client->isPasswordCorrect(password)) {
        client->resetIncorrectLogins();
        return client;
    }

    client->increaseIncorrectLogins();
    if (client->isAccountLocked())
        throw LoginFailedError(
            "Wprowadzono niepoprawne haslo. Konto zostalo zablokowane"
        );

    auto count = std::to_string(client->getIncorrectLogins());
    throw LoginFailedError(
        "Wprowadzono niepoprawne haslo " + count + " raz(y) z rzedu. Po 5 "
        + "nieudanych probach logowania konto zostanie zablokowane."
    );
}

std::shared_ptr<Client> ClientManager::findClient(const Predicate<std::shared_ptr<Client>>& predicate) {
    return repository.findFirst(predicate);
}

std::vector<std::shared_ptr<Client>> ClientManager::findAllClients(
    const Predicate<std::shared_ptr<Client>>& predicate
) {
    return repository.findAll(predicate);
}

const std::vector<std::shared_ptr<Client>> & ClientManager::getAllClients() {
    return repository.getAll();
}

bool ClientManager::unregisterClient(const std::string& personalID) {
    auto client = getClient(personalID);
    if (!client)
        return false;

    repository.remove(client);
    return true;
}

void ClientManager::save() {
    // Najpierw usuwamy wszystkie pliki (aby pliki usuniętych podczas
    // działania programu obiektów zostały usunięte)
    storageHandler.removeAll();

    for (const auto& client : repository.getAll())
        storageHandler.saveObject(client);
}

void ClientManager::load() {
    repository.removeAll();

    auto objects = storageHandler.getAllObjects();
    for (const auto& [id, data] : objects) {
        auto client = Client::fromJSON(data);
        repository.add(client);
    }
}
