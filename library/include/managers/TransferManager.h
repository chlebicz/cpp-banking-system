#pragma once
#include "Repository.h"
#include "StorageHandler.h"
#include "model/Transfer.h"

class ClientManager;
class AccountManager;

/**
 * Exception thrown when a transfer with the given parameters cannot be performed
 */
class InvalidTransferError : public std::logic_error {
public:
    /**
     * Constructor
     * @param msg Message allowing to evaluate the source of the error
     */
    explicit InvalidTransferError(const std::string& msg)
        : std::logic_error(msg) {}
};

/**
 * Transfer Manager
 */
class TransferManager {
public:
    /**
     * Constructor
     * @param clientManager Client manager
     * @param accountManager Account manager
     */
    TransferManager(
        ClientManager& clientManager, AccountManager& accountManager
    );

    /**
     * Registers a transfer with the given parameters
     * @param senderID Account number of the account from which the transfer was sent
     * @param recipientID Recipient's account number
     * @param amount Transfer amount
     * @param type Transfer type
     * @returns Created transfer object
     * @throws NotEnoughMoney When there are not enough funds on the account
     * @throws InvalidTransferError When the transfer has an incorrect type
     */
    std::shared_ptr<Transfer> createTransfer(
        std::string senderID, std::string recipientID, Amount amount
    );

    /**
     * @param transferID Transfer identifier
     * @returns Transfer object with the given identifier
     */
    std::shared_ptr<Transfer> getTransfer(const std::string& transferID);

    /**
     * Finds all transfers (outgoing and incoming) that
     * concern a given user.
     * @param personalId Client's PESEL number
     * @returns Vector with found transfer objects
     */
    std::vector<std::shared_ptr<Transfer>> findUserTransfers(
        const std::string& personalId
    );

    /**
     * Finds all transfers (outgoing and incoming) that
     * concern a given account.
     * @param accountNumber Account number
     * @returns Vector with found transfer objects
     */
    std::vector<std::shared_ptr<Transfer>> findAccountTransfers(
        const std::string& accountNumber
    );

    /**
     * Finds the first transfer satisfying the given predicate
     * @param predicate Predicate
     * @returns Found transfer object
     */
    std::shared_ptr<Transfer> findFirst(
        const Predicate<std::shared_ptr<Transfer>>& predicate
    );

    /**
     * Finds all transfers satisfying the given predicate
     * @param predicate Predicate
     * @returns Vector with found transfer objects
     */
    std::vector<std::shared_ptr<Transfer>> findAll(
        const Predicate<std::shared_ptr<Transfer>>& predicate
    );

    /**
     * @returns All transfer objects
     */
    const std::vector<std::shared_ptr<Transfer>>& getAll();

    /**
     * Loads transfer objects from files to the repository
     */
    void load();

    /**
     * Saves transfer objects from the repository to files
     */
    void save();

    /**
     * Handles incoming external transfers whose data has been
     * saved in appropriate files.
     */
    void handleIncomingExternalTransfers();
private:
    ClientManager& clientManager;
    AccountManager& accountManager;

    Repository<std::shared_ptr<Transfer>> repository;
    StorageHandler storageHandler{"transfers"};
};
