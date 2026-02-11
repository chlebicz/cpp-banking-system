#pragma once
#include "Repository.h"
#include "StorageHandler.h"
#include "model/Client.h"

/**
 * Wyjątek wyrzucany, kiedy logowanie się nie powiedzie
 */
class LoginFailedError : public std::runtime_error {
public:
    /**
     * Konstruktor
     * @param msg Wiadomość pozwalająca ocenić źródło błędu
     */
    explicit LoginFailedError(const std::string& msg);
};

/**
 * Wyjątek wyrzucany, kiedy rejestracja klienta się nie powiedzie
 */
class RegisterFailedError : public std::runtime_error {
public:
    /**
     * Konstruktor
     * @param msg Wiadomość pozwalająca ocenić źródło błędu
     */
    explicit RegisterFailedError(const std::string& msg);
};

/**
 * Menedżer klientów
 */
class ClientManager {
public:
    /**
     * Tworzy obiekt klienta o podanych danych. Jeśli taki klient
     * (o podanym numerze PESEL) istnieje, to zwracany jest obiekt
     * istniejącego już klienta.
     * @param firstName Imię klienta
     * @param lastName Nazwisko klienta
     * @param personalID Numer PESEL klienta
     * @param login Login klienta
     * @param password Hasło klienta
     * @return Utworzony/istniejący obiekt klienta
     */
    std::shared_ptr<Client> registerClient(
        std::string firstName, std::string lastName, std::string personalID,
        std::string login, std::string password
    );

    /**
     * @param personalID Numer PESEL
     * @returns Obiekt klienta o podanym numerze PESEL lub nullptr jeśli nie istnieje.
     */
    std::shared_ptr<Client> getClient(const std::string& personalID);

    /**
     * Wyszukuje klienta o podanym loginie i próbuje się zalogować.
     * @param login Login klienta
     * @param password Hasło klienta
     * @returns Obiekt klienta, jeśli logowanie się powiodło
     * @throws LoginFailedError Kiedy klient o podanym loginie nie istnieje lub
     *                          wprowadzone hasło jest niepoprawne
     */
    std::shared_ptr<Client> login(const std::string& login, const std::string& password);

    /**
     * Wyszukuje klienta spełniającego dany predykat.
     * @param predicate Predykat
     * @returns Znaleziony klient
     */
    std::shared_ptr<Client> findClient(const Predicate<std::shared_ptr<Client>>& predicate);

    /**
     * Wyszukuje wszystkich klientów spełniających podany predykat.
     * @param predicate Predykat
     * @returns Wektor z obiektami znalezionych klientów
     */
    std::vector<std::shared_ptr<Client>> findAllClients(
        const Predicate<std::shared_ptr<Client>>& predicate
    );

    /**
     * @returns Wszystkie obiekty klientów
     */
    const std::vector<std::shared_ptr<Client>>& getAllClients();

    /**
     * Usuwa danego klienta
     * @param personalID Numer PESEL
     * @returns Prawda, jeśli klient został usunięty, fałsz w przeciwnym wypadku
     */
    bool unregisterClient(const std::string& personalID);

    /**
     * Zapisuje obiekty klientów z repozytorium do plików.
     */
    void save();

    /**
     * Ładuje obiekty klientów z plików do repozytorium.
     */
    void load();
private:
    Repository<std::shared_ptr<Client>> repository;
    StorageHandler storageHandler{"clients"};
};
