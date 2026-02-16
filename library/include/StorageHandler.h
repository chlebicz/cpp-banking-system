#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <filesystem>
#include <nlohmann/json.hpp>

#include "Entity.h"

/**
 * Error related to file handling
 */
class StorageError : public std::runtime_error {
public:
    /**
     * Constructor
     * @param message Message allowing to evaluate the source of the error
     */
    explicit StorageError(const std::string& message);
};

/**
 * Class responsible for reading and writing data from/to files
 */
class StorageHandler {
public:
    /**
     * Constructor from the path to the directory with object files
     * @param dir Path to the directory where object files are stored
     */
    StorageHandler(std::filesystem::path dir);

    /**
     * Removes an existing object in the database; if the object does not exist,
     * the function does nothing
     * @param id Object identifier
     */
    void removeObject(const std::string& id);

    /**
     * Removes all objects from the database
     */
    void removeAll();

    /**
     * Removes an existing object in the database; if the object does not exist,
     * the function does nothing
     * @param object Object to remove
     */
    void removeObject(std::shared_ptr<Entity> object);

    /**
     * Obtains the JSON format representation of a given object from a file
     * @param id Object identifier
     * @returns Object content in JSON format
     * @throws StorageError If a read error occurs/an object with the given ID does not exist.
     */
    json getObjectData(const std::string& id) const;

    /**
     * @returns All objects in JSON format saved in files
     */
    std::unordered_map<std::string, json> getAllObjects() const;

    /**
     * Saves a new object/updates an existing one to a file
     * @param object Object to save
     */
    void saveObject(std::shared_ptr<Entity> object);

    /**
     * Saves an object to a file
     * @param id Object identifier
     * @param data Object in JSON format
     */
    void saveObject(const std::string& id, const json& data);
private:
    std::filesystem::path dir;
};
