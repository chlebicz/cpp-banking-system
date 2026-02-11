#include <boost/test/unit_test.hpp>
#include "model/Transfer.h"

// zeby boost mogl wypisac TransferType
std::ostream& operator<<(std::ostream& os, TransferType type);

BOOST_AUTO_TEST_SUITE(TransferTest)

/**
 * @test Sprawdza poprawność konstruktora przelewu
 */
BOOST_AUTO_TEST_CASE(TransferConstructorTest) {
    std::string transferID = "11100101";
    std::string senderID = "1234567";
    std::string recipientID = "1234568";
    Amount amount = 100;
    Amount additionalFee = 1;
    auto transferType = TransferType::Own;

    Transfer transfer{
        transferID, senderID, recipientID, amount, additionalFee, transferType
    };

    BOOST_TEST(transfer.getId() == transferID);
    BOOST_TEST(transfer.getSenderID() == senderID);
    BOOST_TEST(transfer.getRecipientID() == recipientID);
    BOOST_TEST(transfer.getAmount() == amount);
    BOOST_TEST(transfer.getFee() == additionalFee);
    BOOST_TEST(transfer.getType() == transferType);
}

/**
 * @test Sprawdza poprawne tworzenie obiektu przelewu z formatu JSON
 */
BOOST_AUTO_TEST_CASE(TransferFromJSONTest) {
    std::string transferID = "11100101";
    std::string senderID = "1234567";
    std::string recipientID = "1234568";
    Amount amount = 100;
    auto transferType = TransferType::Own;

    Transfer transfer{transferID, senderID, recipientID, amount, 0, transferType};

    auto transfer2 = Transfer::fromJSON(transfer.toJSON());

    BOOST_TEST(transfer2->getId() == transferID);
    BOOST_TEST(transfer2->getSenderID() == senderID);
    BOOST_TEST(transfer2->getRecipientID() == recipientID);
    BOOST_TEST(transfer2->getAmount() == amount);
    BOOST_TEST(transfer2->getType() == transferType);
}

/**
 * @test Sprawdza tworzenie odpowiednich typów przelewu z formatu JSON
 */
BOOST_AUTO_TEST_CASE(TransferTypeFromJSONTest) {
    auto transferType = TransferType::Own;
    json transferJSON = {
        {"id", "1"},
        {"senderID", "2"},
        {"recipientID", "3"},
        {"amount", "1,50 zl"},
        {"additionalFee", "0 zl"},
        {"type", static_cast<int>(transferType)}
    };
    auto transfer = Transfer::fromJSON(transferJSON);
    BOOST_TEST(transfer->getType() == transferType);

    transferType = TransferType::Internal;
    transferJSON["type"] = transferType;
    transfer = Transfer::fromJSON(transferJSON);
    BOOST_TEST(transfer->getType() == transferType);

    transferType = TransferType::OutcomingExternal;
    transferJSON["type"] = transferType;
    transfer = Transfer::fromJSON(transferJSON);
    BOOST_TEST(transfer->getType() == transferType);

    transferType = TransferType::IncomingExternal;
    transferJSON["type"] = transferType;
    transfer = Transfer::fromJSON(transferJSON);
    BOOST_TEST(transfer->getType() == transferType);
}

BOOST_AUTO_TEST_SUITE_END()