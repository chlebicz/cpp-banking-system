#pragma once
#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

/**
 * Abstract class whose objects can be persisted in JSON format,
 * used for saving to files, and presented in a user-friendly form.
 */
class Serializable {
public:
    /**
     * Virtual constructor
     */
    virtual ~Serializable() = default;

    /**
     * @returns Class representation as a JSON object
     */
    virtual json toJSON() const = 0;

    /**
     * @returns Class representation as a string in a format
     *          friendly for reading by the user
     */
    virtual std::string toString() const = 0;
};

// Functions used by the json library to save Serializable objects
// in JSON format
void to_json(json& j, const Serializable& s);
void to_json(json& j, const std::shared_ptr<Serializable>& s);
