#include "StorageHandler.h"
#include <filesystem>
#include <fstream>
#include <iostream>

StorageError::StorageError(const std::string& message)
    : std::runtime_error(message)
{}

StorageHandler::StorageHandler(std::filesystem::path dir)
    : dir(std::move(dir))
{
    if (!exists(this->dir))
        create_directory(this->dir);
}

void StorageHandler::removeObject(const std::string& id) {
    std::filesystem::remove(dir / id);
}

void StorageHandler::removeAll() {
    std::filesystem::remove_all(dir);
}

void StorageHandler::removeObject(std::shared_ptr<Entity> object) {
    removeObject(object->getId());
}

json StorageHandler::getObjectData(const std::string& id) const {
    std::ifstream file{dir / id};
    if (!file.good())
        throw StorageError{"Failed to open file"};

    return json::parse(file);
}

std::unordered_map<std::string, json> StorageHandler::getAllObjects() const {
    std::unordered_map<std::string, json> objects;

    if (!exists(dir))
        return objects;

    for (const auto& dirEntry : std::filesystem::directory_iterator(dir)) {
        auto id = dirEntry.path().stem().string();
        objects[id] = getObjectData(id);
    }

    return objects;
}

void StorageHandler::saveObject(std::shared_ptr<Entity> object) {
    saveObject(object->getId(), object->toJSON());
}

void StorageHandler::saveObject(const std::string& id, const json& data) {
    if (!exists(this->dir))
        create_directory(this->dir);

    std::ofstream file{dir / id};
    if (!file.good())
        throw std::runtime_error{"Failed to open file"};

    file << data;
    file.close();
}
