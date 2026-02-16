#pragma once
#include "Repository.h"
#include "StorageHandler.h"
#include "model/Client.h"

/**
 * Exception thrown when login fails
 */
class LoginFailedError : public std::runtime_error {
public:
    /**
     * Constructor
     * @param msg Message allowing to evaluate the source of the error
     */
    explicit LoginFailedError(const std::string& msg);
};

/**
 * Exception thrown when client registration fails
 */
class RegisterFailedError : public std::runtime_error {
public:
    /**
     * Constructor
     * @param msg Message allowing to evaluate the source of the error
     */
    explicit RegisterFailedError(const std::string& msg);
};

/**
 * Client Manager
 */
class ClientManager {
public:
    /**
     * Creates a client object with the given data. If such a client
     * (with the given PESEL number) exists, an object of the
     * existing client is returned.
     * @param firstName Client's first name
     * @param lastName Client's last name
     * @param personalID Client's PESEL number
     * @param login Client's login
     * @param password Client's password
     * @return Created/existing client object
     */
    std::shared_ptr<Client> registerClient(
        std::string firstName, std::string lastName, std::string personalID,
        std::string login, std::string password
    );

    /**
     * @param personalID PESEL number
     * @returns Client object with the given PESEL number or nullptr if it does not exist.
     */
    std::shared_ptr<Client> getClient(const std::string& personalID);

    /**
     * Finds a client with the given login and attempts to log in.
     * @param login Client's login
     * @param password Client's password
     * @returns Client object if login was successful
     * @throws LoginFailedError When a client with the given login does not exist or
     *                          the entered password is incorrect
     */
    std::shared_ptr<Client> login(const std::string& login, const std::string& password);

    /**
     * Finds a client satisfying the given predicate.
     * @param predicate Predicate
     * @returns Found client
     */
    std::shared_ptr<Client> findClient(const Predicate<std::shared_ptr<Client>>& predicate);

    /**
     * Finds all clients satisfying the given predicate.
     * @param predicate Predicate
     * @returns Vector with objects of found clients
     */
    std::vector<std::shared_ptr<Client>> findAllClients(
        const Predicate<std::shared_ptr<Client>>& predicate
    );

    /**
     * @returns All client objects
     */
    const std::vector<std::shared_ptr<Client>>& getAllClients();

    /**
     * Removes a given client
     * @param personalID PESEL number
     * @returns True if the client was removed, false otherwise
     */
    bool unregisterClient(const std::string& personalID);

    /**
     * Saves client objects from the repository to files.
     */
    void save();

    /**
     * Loads client objects from files to the repository.
     */
    void load();
private:
    Repository<std::shared_ptr<Client>> repository;
    StorageHandler storageHandler{"clients"};
};
