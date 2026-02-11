#pragma once
#include <memory>

#include "Amount.h"
#include "Entity.h"
#include "Serializable.h"

/**
 * Typ przelewu
 */
enum class TransferType {
    Own = 0,
    Internal = 1,
    OutcomingExternal = 2,
    IncomingExternal = 3
};

/**
 * Klasa reprezentująca przelew
 */
class Transfer : public Entity {
public:
    /**
     * Konstruktor
     * @param id Identyfikator przelewu
     * @param senderID Numer rachunku konta wysyłającego przelew
     * @param recipientID Numer rachunku konta odbiorcy przelewu
     * @param amount Kwota przelewu
     * @param type Typ przelewu
     */
    Transfer(
        std::string id, std::string senderID, std::string recipientID,
        Amount amount, Amount additionalFee, TransferType type
    );

    /**
     * @returns Zadeklarowana kwota przelewu
     */
    Amount getAmount() const;
    /**
     * @returns Opłaty dodatkowe za przelew
     */
    Amount getFee() const;

    /**
     * @returns Kwota zabrana z konta przelewającego
     */
    Amount getSentAmount() const;
    /**
     * @returns Kwota przelana na konto docelowe
     */
    Amount getReceivedAmount() const;

    /**
     * @returns Numer rachunku konta, z którego poszły pieniążki
     */
    const std::string& getSenderID() const;
    /**
     * @returns Numer rachunku konta odbiorcy przelewu
     */
    const std::string& getRecipientID() const;

    /**
     * @returns Identyfikator przelewu
     */
    std::string getId() const override;
    /**
     * @returns Dane przelewu w postaci ciągu znaków
     */
    std::string toString() const override;

    /**
     * @returns Wszystkie informacje o danym obiekcie w formacie JSON, które
     *          są potrzebne do otworzenia jego stanu przy odczycie z pliku
     */
    json toJSON() const override;

    /**
     * @returns Typ przelewu
     */
    TransferType getType() const;

    /**
     * Sprawdza, czy przelew został wykonany z/do podanego numeru rachunku
     * @param accountNumber Sprawdzany numer rachunku
     */
    bool concernsAccount(const std::string& accountNumber) const;

    /**
     * Tworzy obiekt przelewu z reprezentacji znakowej
     * @param source Ciąg znaków reprezentujący dane przelewu
     * @returns Utworzony obiekt przelewu
     */
    static std::shared_ptr<Transfer> fromJSON(const json& source);
private:
    std::string id;
    std::string senderID, recipientID;
    Amount amount;
    Amount additionalFee;
    TransferType type;
};
