#include <boost/test/unit_test.hpp>

#include "managers/AccountManager.h"
#include "managers/ClientManager.h"
#include "managers/TransferManager.h"

std::ostream& operator<<(std::ostream& os, TransferType type) {
    os << static_cast<int>(type);
    return os;
}

BOOST_AUTO_TEST_SUITE(TransferManagerTest)

/**
 * @test Checks correct creation of internal transfers
 */
BOOST_AUTO_TEST_CASE(CreateInternalTransferTest) {
    ClientManager clientManager;
    AccountManager accountManager;
    TransferManager transferManager{clientManager, accountManager};

    auto acc1 = accountManager.openAccount("1", AccountType::Main);
    acc1->setBalance(200);
    auto acc2 = accountManager.openAccount("1", AccountType::Main);
    acc2->setBalance(200);

    Amount amount = {100,00};
    auto transfer = transferManager.createTransfer(
        acc1->getId(), acc2->getId(), amount
    );

    BOOST_TEST(transfer->getSenderID() == acc1->getId());
    BOOST_TEST(transfer->getRecipientID() == acc2->getId());

    BOOST_TEST(transfer->getAmount() == amount);
    BOOST_TEST(transfer->getReceivedAmount() == amount);
    BOOST_TEST(transfer->getSentAmount() == amount);
    BOOST_TEST(transfer->getFee() == 0);

    BOOST_TEST(acc1->getBalance() == 100);
    BOOST_TEST(acc2->getBalance() == 300);

    BOOST_TEST(transfer->getType() == TransferType::Own);
}

/**
 * @test Checks throwing NotEnoughMoney exception when there are not enough
 *       funds to perform the transfer
 */
BOOST_AUTO_TEST_CASE(CreateTransferNotEnoughMoneyTest) {
    ClientManager clientManager;
    AccountManager accountManager;
    TransferManager transferManager{clientManager, accountManager};

    auto acc1 = accountManager.openAccount("1", AccountType::Main);
    acc1->setBalance(89);
    auto acc2 = accountManager.openAccount("1", AccountType::Main);
    acc2->setBalance(200);

    Amount amount = {100,00};
    BOOST_REQUIRE_THROW(
        transferManager.createTransfer(
            acc1->getId(), acc2->getId(), amount
        ),
        NotEnoughMoney
    );

    BOOST_TEST(acc1->getBalance() == 89);
    BOOST_TEST(acc2->getBalance() == 200);
}

/**
 * @test Checks correct execution of external transfers (saving to file
 *       for booking)
 */
BOOST_AUTO_TEST_CASE(CreateExternalTransferTest) {
    ClientManager clientManager;
    AccountManager accountManager;
    TransferManager transferManager{clientManager, accountManager};

    auto acc = accountManager.openAccount("1", AccountType::Main);
    acc->setBalance(200);

    StorageHandler sh{"outcoming_external_transfers"};
    sh.removeAll();
    BOOST_REQUIRE(sh.getAllObjects().empty());

    Amount amount = {100,00};

    const std::string recipient = "0000";
    auto transfer = transferManager.createTransfer(
        acc->getId(), recipient, amount
    );

    BOOST_TEST(transfer->getSenderID() == acc->getId());
    BOOST_TEST(transfer->getRecipientID() == recipient);

    BOOST_TEST(transfer->getAmount() == amount);
    BOOST_TEST(transfer->getReceivedAmount() == amount);

    Amount sent = {100,12};
    BOOST_TEST(transfer->getSentAmount() == sent);

    Amount fee = {0,12};
    BOOST_TEST(transfer->getFee() == fee);

    Amount newBalance = {99,88};
    BOOST_TEST(acc->getBalance() == newBalance);

    BOOST_TEST(transfer->getType() == TransferType::OutcomingExternal);
}

/**
 * @test Checks finding transfers that were made by/to a client with
 *       the given PESEL number
 */
BOOST_AUTO_TEST_CASE(FindUserTransfersTest) {
    ClientManager clientManager;
    AccountManager accountManager;
    TransferManager transferManager{clientManager, accountManager};

    std::string name = "Hartosz", lastName = "Borna", login = "urzytkownik",
        password = "duma1234", personalID = "123456789";
    auto client1 = clientManager.registerClient(
        name, lastName, personalID, login, password
    );
    auto client2 = clientManager.registerClient(
        "Chikolaj", "Mlebicz", "131313", "login", "maslo"
    );

    auto acc1 = accountManager.openAccount(client1->getId(), AccountType::Main);
    acc1->setBalance(100);
    auto acc2 = accountManager.openAccount(client2->getId(), AccountType::Main);
    acc2->setBalance(100);

    transferManager.createTransfer(acc1->getId(), "0000", 1);
    transferManager.createTransfer(acc1->getId(), "0000", 1);
    transferManager.createTransfer(acc1->getId(), "0000", 1);
    transferManager.createTransfer(acc2->getId(), "0000", 1);

    BOOST_TEST(transferManager.findUserTransfers(client1->getId()).size() == 3);
    BOOST_TEST(transferManager.findUserTransfers(client2->getId()).size() == 1);
}

/**
 * @test Checks correct finding of transfers from/to the given account
 */
BOOST_AUTO_TEST_CASE(FindAccountTransfersTest) {
    ClientManager clientManager;
    AccountManager accountManager;
    TransferManager transferManager{clientManager, accountManager};

    auto acc1 = accountManager.openAccount("1", AccountType::Main);
    acc1->setBalance(100);
    auto acc2 = accountManager.openAccount("2", AccountType::Main);
    acc2->setBalance(100);

    transferManager.createTransfer(acc1->getId(), "0000", 1);
    transferManager.createTransfer(acc1->getId(), "0000", 1);
    transferManager.createTransfer(acc1->getId(), "0000", 1);
    transferManager.createTransfer(acc2->getId(), "0000", 1);

    BOOST_TEST(transferManager.findAccountTransfers(acc1->getId()).size() == 3);
    BOOST_TEST(transferManager.findAccountTransfers(acc2->getId()).size() == 1);
}

/**
 * @test Checks if after saving a transfer it can be read
 */
BOOST_AUTO_TEST_CASE(TransferManSaveLoadTest) {
    std::string id;

    {
        ClientManager clientManager;
        AccountManager accountManager;
        TransferManager transferManager{clientManager, accountManager};

        auto client1 = clientManager.registerClient(
            "Hartosz", "Borna", "urzytkownik", "duma1234", "123456789"
        );
        auto client2 = clientManager.registerClient(
            "Chikolaj", "Mlebicz", "131313", "login", "maslo"
        );

        auto acc1 = accountManager.openAccount(client1->getId(), AccountType::Main);
        acc1->setBalance(200);
        auto acc2 = accountManager.openAccount(client2->getId(), AccountType::Main);
        acc2->setBalance(100);

        auto transfer = transferManager.createTransfer(acc1->getId(), acc2->getId(), 100);
        id = transfer->getId();
        transferManager.save();
    }

    ClientManager clientManager;
    AccountManager accountManager;
    TransferManager transferManager{clientManager, accountManager};
    transferManager.load();

    BOOST_TEST(transferManager.getAll().size() == 1);
    BOOST_TEST(transferManager.getTransfer(id));
}

/**
 * @test Checks handling of incoming external transfers
 */
BOOST_AUTO_TEST_CASE(HandleIncomingExternalTransfersTest) {
    ClientManager clientManager;
    AccountManager accountManager;
    TransferManager transferManager{clientManager, accountManager};

    auto acc1 = accountManager.openAccount("1", AccountType::Main);

    auto transfer = std::make_shared<Transfer>(
        "1", "0000", acc1->getId(), 100, 0, TransferType::IncomingExternal
    );
    StorageHandler sh{"incoming_external_transfers"};
    sh.removeAll();
    sh.saveObject(transfer);

    transferManager.handleIncomingExternalTransfers();
    BOOST_TEST(transferManager.getTransfer("1"));
    BOOST_TEST(acc1->getBalance() == 100);
    BOOST_TEST(sh.getAllObjects().empty());
}

BOOST_AUTO_TEST_SUITE_END()
