#pragma once
#include <memory>

#include "Amount.h"
#include "Entity.h"
#include "Serializable.h"

/**
 * Transfer type
 */
enum class TransferType {
    Own = 0,
    Internal = 1,
    OutcomingExternal = 2,
    IncomingExternal = 3
};

/**
 * Class representing a transfer
 */
class Transfer : public Entity {
public:
    /**
     * Constructor
     * @param id Transfer ID
     * @param senderID Account number of the sending account
     * @param recipientID Account number of the receiving account
     * @param amount Transfer amount
     * @param type Transfer type
     */
    Transfer(
        std::string id, std::string senderID, std::string recipientID,
        Amount amount, Amount additionalFee, TransferType type
    );

    /**
     * @returns Declared transfer amount
     */
    Amount getAmount() const;
    /**
     * @returns Additional transfer fees
     */
    Amount getFee() const;

    /**
     * @returns Amount taken from the sending account
     */
    Amount getSentAmount() const;
    /**
     * @returns Amount transferred to the destination account
     */
    Amount getReceivedAmount() const;

    /**
     * @returns Account number of the account from which money was sent
     */
    const std::string& getSenderID() const;
    /**
     * @returns Account number of the transfer recipient
     */
    const std::string& getRecipientID() const;

    /**
     * @returns Transfer ID
     */
    std::string getId() const override;
    /**
     * @returns Transfer data as a string
     */
    std::string toString() const override;

    /**
     * @returns All information about the object in JSON format, needed to restore its state when reading from a file
     */
    json toJSON() const override;

    /**
     * @returns Transfer type
     */
    TransferType getType() const;

    /**
     * Checks if the transfer was made from/to the given account number
     * @param accountNumber Checked account number
     */
    bool concernsAccount(const std::string& accountNumber) const;

    /**
     * Creates a transfer object from string representation
     * @param source String representing transfer data
     * @returns Created transfer object
     */
    static std::shared_ptr<Transfer> fromJSON(const json& source);
private:
    std::string id;
    std::string senderID, recipientID;
    Amount amount;
    Amount additionalFee;
    TransferType type;
};
