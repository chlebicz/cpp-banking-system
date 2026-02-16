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
            "Client with the given PESEL number is already registered"
        };

    auto sameLogin = findClient([&login] (const auto& c) {
        return c->getLogin() == login;
    });
    if (sameLogin)
        throw RegisterFailedError{"Client with the given login already exists"};

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
        throw LoginFailedError("Invalid username");

    if (client->isAccountLocked())
        throw LoginFailedError(
            "Your account is locked. Login failed"
        );

    if (client->isPasswordCorrect(password)) {
        client->resetIncorrectLogins();
        return client;
    }

    client->increaseIncorrectLogins();
    if (client->isAccountLocked())
        throw LoginFailedError(
            "Invalid password entered. Account has been locked"
        );

    auto count = std::to_string(client->getIncorrectLogins());
    throw LoginFailedError(
        "Invalid password entered " + count + " times in a row. After 5 "
        + "failed login attempts the account will be locked."
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
    // First remove all files (so that files of objects deleted during
    // the program execution are removed)
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
