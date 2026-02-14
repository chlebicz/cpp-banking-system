#include "UserInterface.h"
#include <iostream>
#include <limits>

#include "model/InvestmentAccount.h"
#include "model/SavingsAccount.h"
#include <termios.h>

UserInterface::UserInterface() {
    Bank::getInstance().loadAll();
}

bool UserInterface::renderEntryView() {
    std::cout << "Press r to register an account" << std::endl;
    std::cout << "Press l to login" <<std::endl;
    std::cout << "Press x to exit" << std::endl;
    char press;
    bool val;
    std::cin.get(press);
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    switch (press) {
        case 'r':
            registerClient();
            break;
        case 'l':
            val = login();
            if (!val) {
                break;
            }
            bool status;
            status = loggedClient->payForLoans();
            if (!status) {
                std::cout << "Your account has been locked" << std::endl;
                return false;
            }
            mainLoop();
            break;
        case 'x':
            return false;
        default:
            break;
    }
    return true;
}

bool UserInterface::renderMainView() {
    std::cout << "Press f to make a transfer" << std::endl;
    std::cout << "Press o to order a new card" << std::endl;
    std::cout << "Press t to make a card transaction" << std::endl;
    std::cout << "Press i to display information about your accounts" << std::endl;
    std::cout << "Press k to open a new account" << std::endl;
    std::cout << "Press j to display information about loans" << std::endl;
    std::cout << "Press q to take a loan" << std::endl;
    std::cout << "Press b to display information about a deposit" << std::endl;
    std::cout << "Press d to open a deposit" << std::endl;
    std::cout << "Press z to end a deposit" << std::endl;
    std::cout << "Press m to buy gold coins (investment account required)" << std::endl;
    std::cout << "Press s to sell owned gold coins" << std::endl;
    std::cout << "Press h to change password" << std::endl;
    std::cout << "Press x to exit" << std::endl;
    char press;
    std::cin.get(press);
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    switch (press) {
        case 'f':
            createTransfer();
            break;
        case 'o':
            orderNewCard();
            break;
        case 't':
            transaction();
            break;
        case 'i':
            checkAccounts();
            break;
        case 'k':
            openAccount();
            break;
        case 'j':
            loanInfo();
            break;
        case 'q':
            takeLoan();
            break;
        case 'b':
            depositInfo();
            break;
        case 'd':
            createDeposit();
            break;
        case 'z':
            endDeposit();
            break;
        case 'm':
            buyGold();
            break;
        case 's':
            sellGold();
            break;
        case 'h':
            changePassword();
            break;
        case 'x':
            return false;
        default:
            break;
    }
    return true;
}

void UserInterface::renderBannedView() {
    std::cout << "Your account has been locked, press x to exit" << std::endl;
}

void UserInterface::entryLoop() {
    bool running = true;
    while (running) {
        running = renderEntryView();
    }
}

void UserInterface::mainLoop() {
    bool running = true;
    while (running) {
        running = renderMainView();
    }
}

void UserInterface::openAccount() {
    std::cout << "Press a if this is to be a main account" << std::endl;
    std::cout << "Press b if this is to be a crypto account" << std::endl;
    std::cout << "Press c if this is to be a savings account" << std::endl;
    std::cout << "Press d if this is to be an investment account" << std::endl;
    char press;
    std::cin.get(press);
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    AccountType accountType;
    switch (press) {
        case 'a':
            accountType = AccountType::Main;
            break;
        case 'b':
            accountType = AccountType::Crypto;
            break;
        case 'c':
            accountType = AccountType::Savings;
            break;
        case 'd':
            accountType = AccountType::Investment;
        break;
        default:
            std::cout << "Failed to create account" << std::endl;
            return;
    }
    Bank::getInstance().openAccount(loggedClient->getPersonalId(), accountType);
    std::cout << "Account created successfully" << std::endl;
}

void UserInterface::checkAccounts() {
    for (auto account: Bank::getInstance().checkAccounts(loggedClient->getPersonalId())) {
        std::cout << account->toString() << std::endl;
    }
    std::cout << std::endl;
}

void UserInterface::takeLoan() {
    int months;
    int intLoanAmount;
    std::string loanAccount;
    std::cout << "Enter loan duration in months: " << std::endl;
    std::cin >> months;
    if (cinCheck()) {
        return;
    }
    std::cout << "Enter loan amount (whole zl): " << std::endl;
    std::cin >> intLoanAmount;
    if (cinCheck()) {
        return;
    }
    Amount loanAmount = intLoanAmount;
    std::cout << "Your accounts:" << std::endl;
    checkAccounts();
    std::cout << "Choose the operational account number for the loan: " << std::endl;
    std::cin >> loanAccount;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    try {
        Bank::getInstance().takeLoan(months, loanAmount, loanAccount, loggedClient->getPersonalId());
        std::cout << "Loan taken successfully" << std::endl;
    } catch (InvalidAccountError & e) {
        std::cout << e.what() << std::endl;
    }
}

void UserInterface::registerClient() {
    std::string name;
    std::string lastName;
    std::string personalId;
    std::string login;
    std::cout << "Enter first name: " << std::endl;
    std::cin >> name;
    std::cout << "Enter last name: " << std::endl;
    std::cin >> lastName;
    std::cout << "Enter PESEL: " << std::endl;
    std::cin >> personalId;
    std::cout << "Enter login: " << std::endl;
    std::cin >> login;
    std::string password = passwordInput("Enter password: ");

    try {
        Bank::getInstance().getClientManager().registerClient(
            name, lastName, personalId, login, password
        );

        std::cout << "Registered successfully" << std::endl;
    } catch(const RegisterFailedError& e) {
        std::cerr << e.what() << std::endl;
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

bool UserInterface::login() {
    std::string login;
    std::cout << "Enter login: ";
    std::cin >> login;

    std::string password = passwordInput("Enter password: ");

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    try {
        loggedClient = Bank::getInstance().getClientManager().login(
            login, password
        );
    } catch (const LoginFailedError& e) {
        std::cerr << e.what() << std::endl;
        return false;
    }

    std::cout << "Logged in as " + loggedClient->getLogin() << std::endl;
    return true;
}

void UserInterface::changePassword() {
    std::string password = passwordInput("Enter current password: ");

    if (!loggedClient->isPasswordCorrect(password)) {
        std::cerr << "Incorrect password entered" << std::endl;
        return;
    }

    password = passwordInput("Enter new password: ");

    loggedClient->setPassword(password);
    std::cout << "Password has been changed" << std::endl;
}

void UserInterface::orderNewCard() {
    std::string account;
    std::cout << "Enter account no.: " << std::endl;
    std::cin >> account;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Press a if this is to be a standard card (free)" << std::endl;
    std::cout << "Press b if this is to be a gold card (cost 100 zl)" << std::endl;
    std::cout << "Press c if this is to be a diamond card (cost 500 zl)" << std::endl;
    char press;
    std::cin.get(press);
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    CardType cardType;
    switch (press) {
        case 'a':
            cardType = CardType::Standard;
            break;
        case 'b':
            cardType = CardType::Gold;
            break;
        case 'c':
            cardType = CardType::Diamond;
            break;
        default:
            return;
    }

    try {
        Bank::getInstance().orderNewCard(account, cardType);
    } catch (const InvalidAmountError&) {
        std::cout << "You have insufficient funds to order this card" << std::endl;
    }
}

void UserInterface::createDeposit() {
    std::string account;
    int intAmount;
    std::cout << "Enter account no.: " << std::endl;
    std::cin >> account;
    std::cout << "Enter deposit amount (whole zl): " << std::endl;
    std::cin >> intAmount;
    if (cinCheck()) {
        return;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    Amount amount = intAmount;
    bool status = Bank::getInstance().createDeposit(account, amount);
    if (status) {
        std::cout << "Deposit created successfully" << std::endl;
        return;
    }
    std::cout << "Failed to create deposit, check if it is a savings account" << std::endl;
}

void UserInterface::endDeposit() {
    std::string account;
    std::cout << "Enter account no.: " << std::endl;
    std::cin >> account;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    Bank::getInstance().endDeposit(account);
}

void UserInterface::loanInfo() {
    std::cout << Bank::getInstance().loanInfo(loggedClient->getPersonalId()) << std::endl;
}

void UserInterface::depositInfo() {
    Bank& bank = Bank::getInstance();
    AccountManager& accountManager = bank.getAccountManager();

    auto accounts = accountManager.findSavingsAccounts(
        loggedClient->getId()
    );

    if (accounts.empty()) {
        std::cerr << "You do not have a savings account" << std::endl;
        return;
    }

    auto pickedAccount = accounts[0];
    if (accounts.size() > 1) {
        std::cout << "Choose a savings account:" << std::endl;
        for (int i = 0; i < accounts.size(); ++i)
            std::cout << i + 1 << ". " << accounts[i]->toString() << std::endl;

        unsigned int pickedNumber;
        std::cin >> pickedNumber;
        if (cinCheck() || pickedNumber > accounts.size())
            return;

        auto index = pickedNumber - 1;
        pickedAccount = accounts[index];
    }

    if (!pickedAccount->getDeposit()) {
        std::cerr << "You do not have a deposit" << std::endl;
        return;
    }

    std::cout << pickedAccount->getDeposit()->toString() << std::endl;
}

void UserInterface::createTransfer() {
    std::cout << "Your accounts: " << std::endl;
    checkAccounts();

    std::cout << "Enter the account number from which to make the transfer: ";
    std::string senderID;
    std::cin >> senderID;

    if (cinCheck()) return;

    Bank& bank = Bank::getInstance();
    auto& accountManager = bank.getAccountManager();
    if (!accountManager.isClientsAccount(loggedClient->getId(), senderID)) {
        std::cerr << "Cannot use this account" << std::endl;
        return;
    }

    std::cout << "Enter the recipient's account number (you can also enter "
        << "the number of one of your accounts): " << std::endl;
    std::string recipientID;
    std::cin >> recipientID;

    if (cinCheck()) return;

    std::cout << "Enter transfer amount: " << std::endl;
    std::string amountStr;
    std::cin >> amountStr;

    if (cinCheck()) return;

    Amount amount = 0;
    try {
        amount = Amount::fromString(amountStr);
    } catch(const InvalidAmountError& e) {
        std::cerr << e.what() << std::endl;
        return;
    }

    auto& transferManager = Bank::getInstance().getTransferManager();

    try {
        transferManager.createTransfer(
            senderID, recipientID, amount
        );
    } catch (const NotEnoughMoney& e) {
        std::cerr << e.what() << std::endl;
        return;
    }

    std::cout << "Transfer executed :~~)" << std::endl;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void UserInterface::transaction() {
    std::string account;
    std::cout << "Enter sender's account no.: " << std::endl;
    std::cin >> account;
    std::string recipentAccount;
    std::cout << "Enter recipient's account no.: " << std::endl;
    std::cin >> recipentAccount;
    std::cout << "Enter transaction amount: " << std::endl;
    std::string amountStr;
    std::cin >> amountStr;

    if (cinCheck()) return;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    Amount amount = 0;
    try {
        amount = Amount::fromString(amountStr);
    } catch(const InvalidAmountError& e) {
        std::cerr << e.what() << std::endl;
        return;
    }
    bool status =  Bank::getInstance().transaction(account, recipentAccount, amount);
    if (!status) {
        std::cout << "Transaction failed" << std::endl;
    }
}

bool UserInterface::cinCheck() {
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid format" << std::endl;
        return true;
    }
    return false;
}

void UserInterface::buyGold() {
    Bank& bank = Bank::getInstance();
    AccountManager& accountManager = bank.getAccountManager();

    auto accounts = accountManager.findInvestmentAccounts(
        loggedClient->getId()
    );

    if (accounts.empty()) {
        std::cerr << "You do not have an investment account" << std::endl;
        return;
    }

    auto pickedAccount = accounts[0];
    if (accounts.size() > 1) {
        std::cout << "Choose an investment account:" << std::endl;
        for (int i = 0; i < accounts.size(); ++i)
            std::cout << i + 1 << ". " << accounts[i]->toString() << std::endl;

        int pickedNumber;
        std::cin >> pickedNumber;
        if (cinCheck() || pickedNumber <= 0 || pickedNumber > accounts.size())
            return;

        auto index = pickedNumber - 1;
        pickedAccount = accounts[index];
    }

    std::cout << "Enter the number of gold coins you want to buy: ";
    int count;
    std::cin >> count;
    if (cinCheck() || count <= 0) return;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    try {
        pickedAccount->buyGold(count);
        std::cout << "Gold coins purchased" << std::endl;
    } catch(const NotEnoughMoney& e) {
        std::cerr << e.what() << std::endl;
    } catch(const GoldError& e) {
        std::cerr << e.what() << std::endl;
    }
}

void UserInterface::sellGold() {
    Bank& bank = Bank::getInstance();
    AccountManager& accountManager = bank.getAccountManager();

    auto accounts = accountManager.findInvestmentAccounts(
        loggedClient->getId()
    );

    if (accounts.empty()) {
        std::cerr << "You do not have an investment account" << std::endl;
        return;
    }

    auto pickedAccount = accounts[0];
    if (accounts.size() > 1) {
        std::cout << "Choose an investment account:" << std::endl;
        for (int i = 0; i < accounts.size(); ++i)
            std::cout << i + 1 << ". " << accounts[i]->toString() << std::endl;

        int pickedNumber;
        std::cin >> pickedNumber;
        if (cinCheck() || pickedNumber <= 0 || pickedNumber > accounts.size())
            return;

        auto index = pickedNumber - 1;
        pickedAccount = accounts[index];
    }

    try {
        Amount earned = pickedAccount->sellGold();
        std::cout << "Gold coins sold. Profit: " << earned << std::endl;
    } catch(const GoldError& e) {
        std::cerr << e.what() << std::endl;
    }
}

UserInterface::~UserInterface() {
    Bank::getInstance().saveAll();
}

std::string UserInterface::passwordInput(const std::string& prompt) {
    std::cout << prompt;
    std::cout.flush();

    std::string input;
    char currentChar;

    if (!setRawMode(true)) {
        std::getline(std::cin, input);
        return input;
    }

    while (read(STDIN_FILENO, &currentChar, 1) == 1) {
        if (currentChar == 127 && !input.empty()) { // backspace
            input.pop_back();
            std::cout << "\b \b";
            std::cout.flush();
        }
        if (currentChar == 10) // enter
            break;

        if (!isprint(currentChar))
            continue;

        input += currentChar;
        std::cout << "*";
        std::cout.flush();
    }

    std::cout << std::endl;
    setRawMode(false);

    return input;
}

bool UserInterface::setRawMode(bool enabled) {
    static termios oldTerminal, newTerminal;
    if (!enabled)
        return tcsetattr(STDIN_FILENO, TCSANOW, &oldTerminal) == 0;

    tcgetattr(STDIN_FILENO, &oldTerminal);
    newTerminal = oldTerminal;
    newTerminal.c_lflag &= ~(ICANON | ECHO);
    return tcsetattr(STDIN_FILENO, TCSANOW, &newTerminal) == 0;
}
