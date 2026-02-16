#include "managers/TransferManager.h"

#include "RandomDataGenerator.h"
#include "managers/AccountManager.h"
#include "model/Bank.h"

TransferManager::TransferManager(
    ClientManager& clientManager, AccountManager& accountManager
)
    : clientManager(clientManager), accountManager(accountManager)
{}

std::shared_ptr<Transfer> TransferManager::createTransfer(
    std::string senderID, std::string recipientID, Amount amount
) {
    auto id = RandomDataGenerator::generateID();
    while (getTransfer(id))
        id = RandomDataGenerator::generateID();

    auto sender = accountManager.getAccount(senderID);
    if (!sender)
        throw InvalidTransferError{"Invalid sender"};

    TransferType type;
    if (accountManager.isClientsAccount(sender->getOwnerID(), recipientID))
        type = TransferType::Own;
    else if (accountManager.getAccount(recipientID))
        type = TransferType::Internal;
    else
        type = TransferType::OutcomingExternal;

    auto transfer = std::make_shared<Transfer>(
        std::move(id), std::move(senderID), std::move(recipientID),
        amount, sender->getFee(), type
    );

    Amount newSenderBalance = sender->getBalance();
    try {
        newSenderBalance -= transfer->getSentAmount();
    } catch (const InvalidAmountError& e) {
        throw NotEnoughMoney{"No assets - there are exceptions"};
    }

    sender->setBalance(newSenderBalance);

    if (type == TransferType::Own || type == TransferType::Internal) {
        auto recipient = accountManager.getAccount(transfer->getRecipientID());

        recipient->setBalance(
            recipient->getBalance() + transfer->getReceivedAmount()
        );

        Bank& bank = Bank::getInstance();
        bank.increaseBalance(transfer->getFee());
    } else {
        static StorageHandler oeTransferStorage{"outcoming_external_transfers"};
        oeTransferStorage.saveObject(transfer);
    }

    repository.add(transfer);

    return transfer;
}

std::shared_ptr<Transfer> TransferManager::getTransfer(
    const std::string& transferID
) {
    return repository.findFirst([&transferID](const auto& transfer) {
        return transfer->getId() == transferID;
    });
}

std::vector<std::shared_ptr<Transfer>> TransferManager::findUserTransfers(
    const std::string& personalId
) {
    auto clientAccounts = accountManager.findClientAccounts(personalId);

    auto concernsClient = [&clientAccounts] (
        const std::shared_ptr<Transfer>& transfer
    ) {
        return [&transfer, &clientAccounts] (const auto& account) {
            return transfer->concernsAccount(account->getId());
        };
    };

    auto isUserTransfer =
        [&clientAccounts, &concernsClient](const auto& transfer) {
            return std::any_of(
                clientAccounts.begin(),
                clientAccounts.end(),
                concernsClient(transfer)
            );
        };

    return repository.findAll(isUserTransfer);
}

std::vector<std::shared_ptr<Transfer>> TransferManager::findAccountTransfers(
    const std::string& accountNumber
) {
    return repository.findAll([&accountNumber](const auto& transfer) {
        return transfer->concernsAccount(accountNumber);
    });
}

std::shared_ptr<Transfer> TransferManager::findFirst(
    const Predicate<std::shared_ptr<Transfer>>& predicate
) {
    return repository.findFirst(predicate);
}

std::vector<std::shared_ptr<Transfer>> TransferManager::findAll(
    const Predicate<std::shared_ptr<Transfer>>& predicate
) {
    return repository.findAll(predicate);
}

const std::vector<std::shared_ptr<Transfer>>& TransferManager::getAll() {
    return repository.getAll();
}

void TransferManager::load() {
    repository.removeAll();

    auto objects = storageHandler.getAllObjects();
    for (const auto& [id, data] : objects) {
        auto transfer = Transfer::fromJSON(data);
        repository.add(transfer);
    }
}

void TransferManager::save() {
    // First remove all files (so that files of objects deleted during
    // the program execution are removed)
    storageHandler.removeAll();

    for (const auto& transfer : repository.getAll())
        storageHandler.saveObject(transfer);
}

void TransferManager::handleIncomingExternalTransfers() {
    StorageHandler ieTransferStorage{"incoming_external_transfers"};
    auto ieTransfers = ieTransferStorage.getAllObjects();

    for (const auto& [id, data] : ieTransfers) {
        auto transfer = Transfer::fromJSON(data);
        repository.add(transfer);

        // add money to recipient
        auto recipient = accountManager.getAccount(transfer->getRecipientID());
        recipient->setBalance(recipient->getBalance() + transfer->getAmount());

        // external transfer handled, remove from external database
        ieTransferStorage.removeObject(id);
    }
}
