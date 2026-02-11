#include "model/Bank.h"

#include "model/DiamondCard.h"
#include "model/GoldCard.h"
#include "model/SavingsAccount.h"
#include "model/StandardCard.h"

InvalidAccountError::InvalidAccountError(const std::string& msg)
    : std::logic_error(msg) {}

Bank& Bank::getInstance() {
    static Bank instance;
    return instance;
}

Amount Bank::getBalance() const {
    return balance;
}

void Bank::setBalance(Amount value) {
    balance = value;
}

void Bank::increaseBalance(Amount by) {
    balance += by;
}

void Bank::decreaseBalance(Amount by) {
    try {
        balance -= by;
    } catch(const InvalidAmountError&) {
        throw Bankruptcy{"Bank oglosil bankructwo. Do widzenia."};
    }
}

ClientManager& Bank::getClientManager() {
    return clientManager;
}

TransferManager& Bank::getTransferManager() {
    return transferManager;
}

AccountManager& Bank::getAccountManager() {
    return accountManager;
}

void Bank::loadAll() {
    clientManager.load();
    accountManager.load();
    transferManager.load();
}

void Bank::saveAll() {
    clientManager.save();
    accountManager.save();
    transferManager.save();
}

void Bank::openAccount(std::string clientID, AccountType accountType) {
    accountManager.openAccount(clientID, accountType);
}

std::vector<std::shared_ptr<Account>> Bank::checkAccounts(std::string clientID) {
    return accountManager.findClientAccounts(clientID);
}

void Bank::takeLoan(
    int months, Amount amount, std::string number, std::string clientID
) {
    std::shared_ptr<Account> account = accountManager.getAccount(number);
    if (!account)
        throw InvalidAccountError{"Nie istnieje takie konto!"};
    if (!accountManager.isClientsAccount(clientID, account->getId()))
        throw InvalidAccountError{"To konto nie nalezy do ciebie!"};

    std::shared_ptr<Client> client = clientManager.getClient(clientID);
    client->takeLoan(months, amount, account);
}

void Bank::orderNewCard(std::string number, CardType cardType) {
    auto account = accountManager.getAccount(number);
    if (!account) {
        return;
    }
    std::shared_ptr<Card> card;
    switch (cardType) {
        case CardType::Standard:
            card = std::make_shared<StandardCard>();
        break;
        case CardType::Gold:
            card = std::make_shared<GoldCard>();
        break;
        case CardType::Diamond:
            card = std::make_shared<DiamondCard>();
        break;
    }
    account->addCard(card);
}

bool Bank::createDeposit(std::string number, Amount amount) {
    auto account = std::dynamic_pointer_cast<SavingsAccount>(
        accountManager.getAccount(number)
    );
    if (!account) {
        return false;
    }
    return account->createDeposit(amount);
}

void Bank::endDeposit(std::string number) {
    auto account = std::dynamic_pointer_cast<SavingsAccount>(
        accountManager.getAccount(number)
    );
    if (!account) {
        return;
    }
    account->deleteDeposit();
}

std::string Bank::loanInfo(std::string clientID) {
    std::shared_ptr<Client> client = clientManager.getClient(clientID);
    return client->loansInfo();
}

std::string Bank::depositInfo(std::string number) {
    auto account = std::static_pointer_cast<SavingsAccount>(
        accountManager.getAccount(number)
    );
    std::shared_ptr<Deposit> deposit = account->getDeposit();
    return deposit->toString();
}

bool Bank::transaction(std::string number, std::string number2, Amount amount) {
    auto account = accountManager.getAccount(number);
    auto recipentAccount = accountManager.getAccount(number2);
    if (!account || !recipentAccount)
        return false;

    return account->transaction(amount, recipentAccount);
}

Bank::Bank()
    : transferManager(clientManager, accountManager)
{
    StorageHandler storageHandler{"global"};

    json bankData;
    try {
        bankData = storageHandler.getObjectData("bank");
    } catch(const StorageError&) {
        bankData = {{"balance", "10000000 zl"}};
        storageHandler.saveObject("bank", bankData);
    }

    if (!bankData.contains("balance") || !bankData["balance"].is_string())
        bankData = {{"balance", "10000000 zl"}};

    try {
        balance = Amount::fromString(bankData["balance"]);
    } catch(const InvalidAmountError&) {}
}

Bank::~Bank() {
    StorageHandler storageHandler{"global"};
    storageHandler.saveObject("bank", {{"balance", balance.toJSON()}});
}
