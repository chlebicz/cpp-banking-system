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
     * Konstuktor z wszystkich danych klienta
     * @param name Imię klienta
     * @param lastName Nazwisko klienta
     * @param personalID PESEL klienta
     * @param login Login klienta
     * @param password Hasło klienta
     * @param loans Kredyty klienta
     * @param incorrectLogins Liczba ostatnich nieudanych prób zalogowania
     */
    Client(
        std::string name, std::string lastName, std::string personalID,
        std::string login, std::string password,
        std::vector<std::shared_ptr<Loan>> loans, short incorrectLogins
    );

    /**
     * Konstruktor parametrowy
     * @param name Imię klienta
     * @param lastName Nazwisko klienta
     * @param personalId PESEL klienta
     * @param login Login klienta
     * @param password Hasło klienta
     */
    Client(
        std::string name, std::string lastName, std::string personalId,
        std::string login, std::string password
    );

    /**
     * Getter imienia
     * @return Zwraca imię klienta
     */
    const std::string& getName() const;

    /**
     * Getter nazwiska
     * @return Zwraca nazwisko klienta
     */
    const std::string& getLastName() const;

    /**
     * Getter PESELu
     * @return Zwraca PESEL
     */
    const std::string& getPersonalId() const;

    /**
     * Getter loginu
     * @return Zwraca login
     */
    const std::string& getLogin() const;

    /**
     * Getter hasła
     * @return Zwraca hasło
     * @deprecated Użyj Client::isPasswordCorrect
     */
    const std::string& getPassword() const;

    /**
     * Sprawdza, czy wprowadzone hasło jest poprawne.
     * @param password Wprowadzone hasło
     * @returns Prawda, jeśli wprowadzone hasło zgadza się z hasłem klienta,
     *          w przeciwnym wypadku fałsz
     */
    bool isPasswordCorrect(const std::string& password) const;

    /**
     * @return Istotne informacje o danym obiekcie zapisane w formacie
     *         przyjaznym użytkownikowi
     */
    std::string toString() const override;

    /**
     * @returns Wszystkie informacje o danym obiekcie w formacie JSON, które
     *          są potrzebne do otworzenia jego stanu przy odczycie z pliku
     */
    json toJSON() const override;

    /**
     * @returns Unikalny identyfikator obiektu
     */
    std::string getId() const override;

    /**
     * Tworzy obiekt na bazie informacji w formacie JSON
     * @param source Informacje o obiekcie w formacie JSON
     * @returns Utworzony obiekt
     */
    static std::shared_ptr<Client> fromJSON(const json& source);

    /**
     * Setter imienia
     * @param name Imię klienta
     */
    void setName(const std::string &name);

    /**
     * Setter nazwiska
     * @param lastName Imię klienta
     */
    void setLastName(const std::string &lastName);

    /**
     * Setter loginu
     * @param login Login klienta
     */
    void setLogin(const std::string &login);

    /**
     * Setter hasła
     * @param password Hasło klienta
     */
    void setPassword(const std::string &password);

    /**
     * Funkcja umożliwiająca wzięcie kredytu
     * @param months Okres trwania kredytu
     * @param amount Kwota kredytu
     * @param operationalAccount Konto na które klient dostanie pieniądze i
     * z którego pieniądze będą pobierane
     * @param beginTime Data wzięcia kredytu
     */
    void takeLoan(
        int months, Amount amount, std::shared_ptr<Account> operationalAccount,
        gr::date beginTime = gr::day_clock::local_day()
    );

    /**
     * Funkcja umożliwająca opłacenie kredytów, jeśli okres kredytu minął
     * funkcja usunie ten kredyt
     */
    bool payForLoans();

    /// Funkcja zwracająca wektor z atywnymi kredytami
    const std::vector<std::shared_ptr<Loan>>& getLoans() const;

    /**
     * Funkcja zwracająca informacje o kredytach
     * @return Zwraca informacje o kredytach
     */
    std::string loansInfo() const;

    /**
     * @returns Ilość niepoprawnych prób zalogowania od ostatniej pomyślnej
     *          próby zalogowania
     */
    short getIncorrectLogins() const;

    /**
     * Resetuje ilość niepoprawnych prób zalogowania
     */
    void resetIncorrectLogins();

    /**
     * @return Czy konto klienta jest zablokowane
     */
    bool isAccountLocked() const;

    /**
     * Zwiększa ilość niepoprawnych prób zalogowania
     */
    void increaseIncorrectLogins();
};
