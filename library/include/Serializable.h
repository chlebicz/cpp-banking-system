#pragma once
#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

/**
 * Klasa abstrakcyjna, której obiekty da się utrwalić w formacie JSON,
 * używanym do zapisu do plików, i przedstawić w formie przyjaznej użytkownikowi.
 */
class Serializable {
public:
    /**
     * Konstruktor wirtualny
     */
    virtual ~Serializable() = default;

    /**
     * @returns Reprezentacja klasy w postaci obiektu JSON
     */
    virtual json toJSON() const = 0;

    /**
     * @returns Reprezentacja klasy w postaci ciągu znaków w formacie
     *          przyjaznym do odczytu przez użytkownika
     */
    virtual std::string toString() const = 0;
};

// Funkcje używane przez bibliotekę json do zapisu obiektów Serializable
// w formacie JSON
void to_json(json& j, const Serializable& s);
void to_json(json& j, const std::shared_ptr<Serializable>& s);
