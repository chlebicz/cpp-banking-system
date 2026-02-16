#pragma once
#include <string>
#include <memory>
#include <vector>

#include "Entity.h"
#include "Loan.h"

class Client : public Entity {
private:
    std::string name;
    std::string lastName;
    std::string personalId;
    std::string login;
    std::string password;
    std::vector<std::shared_ptr<Loan>> loans;
    short incorrectLogins = 0;
public:
    /**
     * Constructor from all client data
     * @param name Client's first name
     * @param lastName Client's last name
     * @param personalID Client's PESEL
     * @param login Client's login
     * @param password Client's password
     * @param loans Client's loans
     * @param incorrectLogins Number of recent failed login attempts
     */
    Client(
        std::string name, std::string lastName, std::string personalID,
        std::string login, std::string password,
        std::vector<std::shared_ptr<Loan>> loans, short incorrectLogins
    );

    /**
     * Parameterized constructor
     * @param name Client's first name
     * @param lastName Client's last name
     * @param personalId Client's PESEL
     * @param login Client's login
     * @param password Client's password
     */
    Client(
        std::string name, std::string lastName, std::string personalId,
        std::string login, std::string password
    );

    /**
     * Name getter
     * @return Returns client's first name
     */
    const std::string& getName() const;

    /**
     * Last name getter
     * @return Returns client's last name
     */
    const std::string& getLastName() const;

    /**
     * PESEL getter
     * @return Returns PESEL
     */
    const std::string& getPersonalId() const;

    /**
     * Login getter
     * @return Returns login
     */
    const std::string& getLogin() const;

    /**
     * Password getter
     * @return Returns password
     * @deprecated Use Client::isPasswordCorrect
     */
    const std::string& getPassword() const;

    /**
     * Checks if the entered password is correct.
     * @param password Entered password
     * @returns True if the entered password matches the client's password,
     *          otherwise false
     */
    bool isPasswordCorrect(const std::string& password) const;

    /**
     * @return Significant information about the object saved in a user-friendly format
     */
    std::string toString() const override;

    /**
     * @returns All information about the object in JSON format, needed to restore its state when reading from a file
     */
    json toJSON() const override;

    /**
     * @returns Unique object identifier
     */
    std::string getId() const override;

    /**
     * Creates an object based on information in JSON format
     * @param source Information about the object in JSON format
     * @returns Created object
     */
    static std::shared_ptr<Client> fromJSON(const json& source);

    /**
     * Name setter
     * @param name Client's first name
     */
    void setName(const std::string &name);

    /**
     * Last name setter
     * @param lastName Client's last name
     */
    void setLastName(const std::string &lastName);

    /**
     * Login setter
     * @param login Client's login
     */
    void setLogin(const std::string &login);

    /**
     * Password setter
     * @param password Client's password
     */
    void setPassword(const std::string &password);

    /**
     * Function enabling taking a loan
     * @param months Loan duration
     * @param amount Loan amount
     * @param operationalAccount Account to which the client will receive money and
     * from which money will be withdrawn
     * @param beginTime Date of taking the loan
     */
    void takeLoan(
        int months, Amount amount, std::shared_ptr<Account> operationalAccount,
        gr::date beginTime = gr::day_clock::local_day()
    );

    /**
     * Function enabling paying off loans, if the loan period has expired
     * the function will remove this loan
     */
    bool payForLoans();

    /// Function returning a vector with active loans
    const std::vector<std::shared_ptr<Loan>>& getLoans() const;

    /**
     * Function returning information about loans
     * @return Returns information about loans
     */
    std::string loansInfo() const;

    /**
     * @returns Number of incorrect login attempts since the last successful
     *          login attempt
     */
    short getIncorrectLogins() const;

    /**
     * Resets the number of incorrect login attempts
     */
    void resetIncorrectLogins();

    /**
     * @return Whether the client's account is locked
     */
    bool isAccountLocked() const;

    /**
     * Increases the number of incorrect login attempts
     */
    void increaseIncorrectLogins();
};
