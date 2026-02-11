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
    std::cout << "Nacisnij r aby zarejestrowac konto" << std::endl;
    std::cout << "Nacisnij l aby sie zalogowac" <<std::endl;
    std::cout << "Nacisnij x aby wyjsc" << std::endl;
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
                std::cout << "Twoje konto zostalo zablokowane" << std::endl;
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
    std::cout << "Nacisnij f aby wykonac transfer" << std::endl;
    std::cout << "Nacisnij o aby zamowic nowa karte" << std::endl;
    std::cout << "Nacisnij t aby wykonac transakcje karta" << std::endl;
    std::cout << "Nacisnij i aby wyswietlic informacje o twoich kontach" << std::endl;
    std::cout << "Nacisnij k aby otworzyc nowe konto" << std::endl;
    std::cout << "Nacisnij j aby wyswietlic informacje o kredytach" << std::endl;
    std::cout << "Nacisnij q aby wziac kredyt" << std::endl;
    std::cout << "Nacisnij b aby wyswietlic informacje o lokacie" << std::endl;
    std::cout << "Nacisnij d aby otworzyc lokate" << std::endl;
    std::cout << "Nacisnij z aby zakonczyc lokate" << std::endl;
    std::cout << "Nacisnij m aby zakupic zlote monety (wymagane konto inwestycyjne)" << std::endl;
    std::cout << "Nacisnij s aby sprzedac posiadane zlote monety" << std::endl;
    std::cout << "Nacisnij h aby zmienic haslo" << std::endl;
    std::cout << "Nacisnij x aby wyjsc" << std::endl;
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
    std::cout << "Twoje konto zostalo zablokowane, nacisnij x aby wyjsc" << std::endl;
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
    std::cout << "Nacisnij a jesli ma to byc konto glowne" << std::endl;
    std::cout << "Nacisnij b jesli ma to byc konto kryptowalutowe" << std::endl;
    std::cout << "Nacisnij c jesli ma to byc konto oszczednosciowe" << std::endl;
    std::cout << "Nacisnij d jesli ma to byc konto inwestycyjne" << std::endl;
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
            std::cout << "Nie udalo sie utworzyc konta" << std::endl;
            return;
    }
    Bank::getInstance().openAccount(loggedClient->getPersonalId(), accountType);
    std::cout << "Udalo sie utworzyc konto" << std::endl;
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
    std::cout << "Podaj dlugosc trwania kredytu w miesiacach: " << std::endl;
    std::cin >> months;
    if (cinCheck()) {
        return;
    }
    std::cout << "Podaj kwote kredytu (cale zl): " << std::endl;
    std::cin >> intLoanAmount;
    if (cinCheck()) {
        return;
    }
    Amount loanAmount = intLoanAmount;
    std::cout << "Twoje konta:" << std::endl;
    checkAccounts();
    std::cout << "Wybierz numer konta operacyjnego dla kredytu: " << std::endl;
    std::cin >> loanAccount;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    try {
        Bank::getInstance().takeLoan(months, loanAmount, loanAccount, loggedClient->getPersonalId());
        std::cout << "Udalo sie wziac kredyt" << std::endl;
    } catch (InvalidAccountError & e) {
        std::cout << e.what() << std::endl;
    }
}

void UserInterface::registerClient() {
    std::string name;
    std::string lastName;
    std::string personalId;
    std::string login;
    std::cout << "Podaj imie: " << std::endl;
    std::cin >> name;
    std::cout << "Podaj nazwisko: " << std::endl;
    std::cin >> lastName;
    std::cout << "Podaj PESEL: " << std::endl;
    std::cin >> personalId;
    std::cout << "Podaj login: " << std::endl;
    std::cin >> login;
    std::string password = passwordInput("Podaj haslo: ");

    try {
        Bank::getInstance().getClientManager().registerClient(
            name, lastName, personalId, login, password
        );

        std::cout << "Zarejestrowano pomyslnie" << std::endl;
    } catch(const RegisterFailedError& e) {
        std::cerr << e.what() << std::endl;
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

bool UserInterface::login() {
    std::string login;
    std::cout << "Podaj login: ";
    std::cin >> login;

    std::string password = passwordInput("Podaj haslo: ");

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    try {
        loggedClient = Bank::getInstance().getClientManager().login(
            login, password
        );
    } catch (const LoginFailedError& e) {
        std::cerr << e.what() << std::endl;
        return false;
    }

    std::cout << "Zalogowano jako " + loggedClient->getLogin() << std::endl;
    return true;
}

void UserInterface::changePassword() {
    std::string password = passwordInput("Wprowadz obecne haslo: ");

    if (!loggedClient->isPasswordCorrect(password)) {
        std::cerr << "Wprowadzono niepoprawne haslo" << std::endl;
        return;
    }

    password = passwordInput("Wprowadz nowe haslo: ");

    loggedClient->setPassword(password);
    std::cout << "Haslo zostalo zmienione" << std::endl;
}

void UserInterface::orderNewCard() {
    std::string account;
    std::cout << "Podaj nr. konta: " << std::endl;
    std::cin >> account;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Nacisnij a jesli ma to byc karta standardowa (za darmo)" << std::endl;
    std::cout << "Nacisnij b jesli ma to byc karta zlota (koszt 100 zl)" << std::endl;
    std::cout << "Nacisnij c jesli ma to byc karta diamentowa (koszt 500 zl)" << std::endl;
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
        std::cout << "Masz za malo srodkow, aby zamowic te karte" << std::endl;
    }
}

void UserInterface::createDeposit() {
    std::string account;
    int intAmount;
    std::cout << "Podaj nr. konta: " << std::endl;
    std::cin >> account;
    std::cout << "Podaj kwote depozytu(cale zl): " << std::endl;
    std::cin >> intAmount;
    if (cinCheck()) {
        return;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    Amount amount = intAmount;
    bool status = Bank::getInstance().createDeposit(account, amount);
    if (status) {
        std::cout << "Udalo sie zalozyc lokate" << std::endl;
        return;
    }
    std::cout << "Nie udalo sie zalozyc lokaty, sprawdz czy jest to konto oszczednosciowe" << std::endl;
}

void UserInterface::endDeposit() {
    std::string account;
    std::cout << "Podaj nr. konta: " << std::endl;
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
        std::cerr << "Nie posiadasz konta oszczednosciowego" << std::endl;
        return;
    }

    auto pickedAccount = accounts[0];
    if (accounts.size() > 1) {
        std::cout << "Wybierz konto oszczednosciowe:" << std::endl;
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
        std::cerr << "Nie posiadasz zalozonej lokaty" << std::endl;
        return;
    }

    std::cout << pickedAccount->getDeposit()->toString() << std::endl;
}

void UserInterface::createTransfer() {
    std::cout << "Twoje konta: " << std::endl;
    checkAccounts();

    std::cout << "Podaj numer rachunku, z ktorego wykonac przelew: ";
    std::string senderID;
    std::cin >> senderID;

    if (cinCheck()) return;

    Bank& bank = Bank::getInstance();
    auto& accountManager = bank.getAccountManager();
    if (!accountManager.isClientsAccount(loggedClient->getId(), senderID)) {
        std::cerr << "Nie mozna na te konto" << std::endl;
        return;
    }

    std::cout << "Podaj numer rachunku do przelewu (mozesz tez podac "
        << "numer ktoregos z twoich kont): " << std::endl;
    std::string recipientID;
    std::cin >> recipientID;

    if (cinCheck()) return;

    std::cout << "Podaj kwote przelewu: " << std::endl;
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

    std::cout << "Wykonano przelew :~~)" << std::endl;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void UserInterface::transaction() {
    std::string account;
    std::cout << "Podaj nr. konta nadawcy: " << std::endl;
    std::cin >> account;
    std::string recipentAccount;
    std::cout << "Podaj nr. konta odbiorcy: " << std::endl;
    std::cin >> recipentAccount;
    std::cout << "Podaj kwote transakcji: " << std::endl;
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
        std::cout << "Transakcja nie powiodla sie" << std::endl;
    }
}

bool UserInterface::cinCheck() {
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Niepoprawny format" << std::endl;
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
        std::cerr << "Nie posiadasz konta inwestycyjnego" << std::endl;
        return;
    }

    auto pickedAccount = accounts[0];
    if (accounts.size() > 1) {
        std::cout << "Wybierz konto inwestycyjne:" << std::endl;
        for (int i = 0; i < accounts.size(); ++i)
            std::cout << i + 1 << ". " << accounts[i]->toString() << std::endl;

        int pickedNumber;
        std::cin >> pickedNumber;
        if (cinCheck() || pickedNumber <= 0 || pickedNumber > accounts.size())
            return;

        auto index = pickedNumber - 1;
        pickedAccount = accounts[index];
    }

    std::cout << "Wprowadz liczbe zlotych monet, ktora chcesz kupic: ";
    int count;
    std::cin >> count;
    if (cinCheck() || count <= 0) return;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    try {
        pickedAccount->buyGold(count);
        std::cout << "Zakupiono zlote monety" << std::endl;
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
        std::cerr << "Nie posiadasz konta inwestycyjnego" << std::endl;
        return;
    }

    auto pickedAccount = accounts[0];
    if (accounts.size() > 1) {
        std::cout << "Wybierz konto inwestycyjne:" << std::endl;
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
        std::cout << "Sprzedano zlote monety. Zysk: " << earned << std::endl;
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
