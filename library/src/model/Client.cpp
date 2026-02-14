#include "model/Client.h"
#include <algorithm>
#include <utility>

Client::Client(
    std::string name, std::string lastName, std::string personalID,
    std::string login, std::string password,
    std::vector<std::shared_ptr<Loan>> loans, short incorrectLogins
)
    : name(std::move(name)), lastName(std::move(lastName)),
      personalId(std::move(personalID)), login(std::move(login)),
      password(std::move(password)), loans(std::move(loans)),
      incorrectLogins(incorrectLogins)
{}

Client::Client(
    std::string name, std::string lastName,
    std::string personalId, std::string login,
    std::string password
)
    : name(std::move(name)), lastName(std::move(lastName)), personalId(std::move(personalId)),
      login(std::move(login)), password(std::move(password))
{}

const std::string& Client::getName() const {
    return name;
}

const std::string& Client::getLastName() const {
    return lastName;
}

const std::string& Client::getPersonalId() const {
    return personalId;
}

const std::string& Client::getLogin() const {
    return login;
}

const std::string& Client::getPassword() const {
    return password;
}

bool Client::isPasswordCorrect(const std::string& password) const {
    return this->password == password;
}

std::string Client::toString() const {
    return "Login: " + login + ", "
        + "Name: " + name + ", " + "Last Name: " + lastName + ", "
        + "PESEL: " + personalId
        + ".\nLoans:\n" + loansInfo();
}

json Client::toJSON() const {
    return {
        {"name", name},
        {"lastName", lastName},
        {"personalId", personalId},
        {"login", login},
        {"password", password},
        {"loans", loans},
        {"incorrectLogins", incorrectLogins}
    };
}

std::string Client::getId() const {
    return personalId;
}

std::shared_ptr<Client> Client::fromJSON(const json& source) {
    // currently this method assumes that the given JSON object has a valid format

    std::vector<std::shared_ptr<Loan>> loans;
    loans.reserve(source["loans"].size());
    for (const auto& loan : source["loans"])
        loans.push_back(Loan::fromJSON(loan));

    return std::make_shared<Client>(
        source["name"],
        source["lastName"],
        source["personalId"],
        source["login"],
        source["password"],
        std::move(loans),
        source["incorrectLogins"]
    );
}

void Client::setName(const std::string &name) {
    this->name = name;
}

void Client::setLastName(const std::string &lastName) {
    this->lastName = lastName;
}

void Client::setLogin(const std::string &login) {
    this->login = login;
}

void Client::setPassword(const std::string &password) {
    this->password = password;
}

void Client::takeLoan(
    int months, Amount amount, std::shared_ptr<Account> operationalAccount,
    gr::date beginTime
) {
    std::shared_ptr<Loan> newLoan = std::make_shared<Loan>(
        months, amount, beginTime, std::move(operationalAccount)
    );
    newLoan->create();
    loans.push_back(newLoan);
}

bool Client::payForLoans() {
    for (auto loan : loans) {
        bool status = loan->takeMoney();
        if (!status) {
            return false;
        }
        if (loan->getRemainingMonths() <= 0) {
            loans.erase(
                std::remove(loans.begin(), loans.end(), loan),
                loans.end()
            );
        }
    }
    return true;
}

const std::vector<std::shared_ptr<Loan>> & Client::getLoans() const {
    return loans;
}

std::string Client::loansInfo() const {
    std::stringstream ss;
    for (auto loan : loans) {
        ss << loan->toString() << std::endl;
    }
    return ss.str();
}

short Client::getIncorrectLogins() const {
    return incorrectLogins;
}

void Client::resetIncorrectLogins() {
    incorrectLogins = 0;
}

bool Client::isAccountLocked() const {
    return incorrectLogins >= 5;
}

void Client::increaseIncorrectLogins() {
    ++incorrectLogins;
}
