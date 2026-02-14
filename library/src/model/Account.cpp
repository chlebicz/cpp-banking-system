#include "model/Account.h"

#include <algorithm>
#include <iostream>

#include "model/Bank.h"
#include "model/CryptoAccount.h"
#include "model/InvestmentAccount.h"
#include "model/MainAccount.h"
#include "model/SavingsAccount.h"
#include "model/StandardCard.h"

Account::Account(const json& source)
    : accountNumber(source["accountNumber"]), ownerID(source["ownerID"]),
      balance(Amount::fromJSON(source["balance"]))
{
    cards.reserve(source["cards"].size());

    for (const auto& cardJSON : source["cards"])
        cards.push_back(Card::fromJSON(cardJSON));
}

Account::Account(std::string accountNumber, std::string ownerID)
    : accountNumber(std::move(accountNumber)), ownerID(std::move(ownerID))
{}

Account::Account(
    std::string accountNumber, std::string ownerID, Amount balance,
    std::vector<std::shared_ptr<Card>> cards
)
    : accountNumber(std::move(accountNumber)), ownerID(std::move(ownerID)),
      cards(std::move(cards)), balance(balance)
{}

Amount Account::getBalance() const {
    return balance;
}

void Account::setBalance(const Amount &value) {
    balance = value;
}

const std::string& Account::getAccountNumber() const {
    return accountNumber;
}
const std::string& Account::getOwnerID() const {
    return ownerID;
}

const std::vector<std::shared_ptr<Card>>& Account::getCards() const {
    return cards;
}

void Account::addCard(const std::shared_ptr<Card>& card) {
    setBalance(getBalance() - card->getPrice());
    cards.push_back(card);
}

void Account::removeCard(const std::shared_ptr<Card> &card) {
    cards.erase(std::remove(cards.begin(), cards.end(), card), cards.end());
}

bool Account::transaction(
    const Amount &amount, const std::shared_ptr<Account> &account
) {
    std::shared_ptr<Card> card = nullptr;

    for (auto searchedCard : cards) {
        if (searchedCard->getPrice().getZloty() == 500) {
            card = searchedCard;
            break;
        }
        if (searchedCard->getPrice().getZloty() == 100) {
            card = searchedCard;
        }
        if (card != nullptr) {
            continue;
        }
        if (searchedCard->getPrice().getZloty() == 0) {
            card = searchedCard;
        }
    }

    if (card == nullptr) {
        return false;
    }

        Amount fee = card->calculateFee(amount);
        Amount totalAmount = amount + fee;

        try {
            setBalance(getBalance() - totalAmount);
        } catch (InvalidAmountError) {
            // throw NotEnoughMoney{"Not enough money to perform transaction"};
            return false;
        }
        Bank::getInstance().increaseBalance(fee);
        account->setBalance(account->getBalance() + amount);

        return true;
}

std::string Account::toString() const {
    std::stringstream ss;
    ss << " Account number: " << accountNumber << " Balance: " << balance;
    return ss.str();
}

std::string Account::getId() const {
    return accountNumber;
}

json Account::toJSON() const {
    return {
        {"accountNumber", accountNumber},
        {"ownerID", ownerID},
        {"balance", balance},
        {"cards", cards}
    };
}

std::shared_ptr<Account> Account::fromJSON(const json& source) {
    AccountType type = source["type"];
    switch (type) {
        case AccountType::Main:
            return MainAccount::fromJSON(source);
        case AccountType::Crypto:
            return CryptoAccount::fromJSON(source);
        case AccountType::Investment:
            return InvestmentAccount::fromJSON(source);
        case AccountType::Savings:
            return SavingsAccount::fromJSON(source);
    }

    throw std::logic_error("Something went wrong..");
}

Amount Account::getFee() {
    return transferFee;
}
