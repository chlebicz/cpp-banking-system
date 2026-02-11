#pragma once
#include <string>

/**
 * Klasa abstrakcyjna, której obiekty można jednoznacznie rozróżnić
 * poprzez unikalny identyfikator
 */
class Identifiable {
public:
    /**
     * Destruktor wirtualny
     */
    virtual ~Identifiable() = default;

    /**
     * @returns Unikalny identyfikator obiektu
     */
    virtual std::string getId() const = 0;
};
