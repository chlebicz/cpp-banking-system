#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <filesystem>
#include <nlohmann/json.hpp>

#include "Entity.h"

/**
 * Bład związany z obsługą plików
 */
class StorageError : public std::runtime_error {
public:
    /**
     * Konstruktor
     * @param message Wiadomość pozwalająca ocenić źródło błędu
     */
    explicit StorageError(const std::string& message);
};

/**
 * Klasa odpowiadająca za zapis i odczyt danych z plików
 */
class StorageHandler {
public:
    /**
     * Konstruktor z ścieżki do katalogu z plikami obiektów
     * @param dir Ścieżka do katalogu, w którym są przechowywane pliki obiektów
     */
    StorageHandler(std::filesystem::path dir);

    /**
     * Usuwa istniejący w bazie obiekt; jeśli obiekt nie istnieje,
     * funkcja nic nie robi
     * @param id Identyfikator obiektu
     */
    void removeObject(const std::string& id);

    /**
     * Usuwa wszystkie obiekty z bazy
     */
    void removeAll();

    /**
     * Usuwa istniejący w bazie obiekt; jeśli obiekt nie istnieje,
     * funkcja nic nie robi
     * @param object Obiekt do usunięcia
     */
    void removeObject(std::shared_ptr<Entity> object);

    /**
     * Uzyskuje reprezentację w formacie JSON danego obiektu z pliku
     * @param id Identyfikator obiektu
     * @returns Zawartość obiektu w formacie JSON
     * @throws StorageError Jeśli wystąpi błąd odczytu/nie istnieje obiekt o podanym ID.
     */
    json getObjectData(const std::string& id) const;

    /**
     * @returns Wszystkie obiekty w formacie JSON zapisane w plikach
     */
    std::unordered_map<std::string, json> getAllObjects() const;

    /**
     * Zapisuje nowy obiekt/aktualizuje istniejący do pliku
     * @param object Obiekt do zapisania
     */
    void saveObject(std::shared_ptr<Entity> object);

    /**
     * Zapisuje obiekt do pliku
     * @param id Identyfikator obiektu
     * @param data Obiekt w formacie JSON
     */
    void saveObject(const std::string& id, const json& data);
private:
    std::filesystem::path dir;
};
