#pragma once
#include <memory>
#include "model/Bank.h"

class UserInterface {
    std::shared_ptr<Client> loggedClient = std::make_shared<Client>("John", "Doe", "15", "John_Doe", "1234");
public:
    UserInterface();
    bool renderEntryView();
    bool renderMainView();
    void renderBannedView();
    void entryLoop();
    void mainLoop();
    void openAccount();
    void checkAccounts();
    void takeLoan();
    void registerClient();
    bool login();
    void changePassword();
    void orderNewCard();
    void createDeposit();
    void endDeposit();
    void loanInfo();
    void depositInfo();
    void createTransfer();
    void transaction();
    bool cinCheck();
    void buyGold();
    void sellGold();
    ~UserInterface();
private:
    std::string passwordInput(const std::string& prompt);
    bool setRawMode(bool enabled);
};
