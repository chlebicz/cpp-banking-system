#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include <functional>
#include "Serializable.h"

/**
 * Predicate for a given type
 */
template<typename T>
using Predicate = std::function<bool (const T&)>;

/**
 * Repository - storing data in memory
 * @tparam T Type managed by the repository
 */
template<typename T>
class Repository {
public:
    /**
    * @param id Identifier of the element in the repository
    * @returns pointer to the element
    */
    T get(int id) {
        if (id >= collection.size())
            return nullptr;
        return collection[id];
    }

    /**
    * Adds an element to the repository
    * @param element pointer to the element to add
    */
    void add(T element) {
        if (!element)
            return;
        collection.push_back(std::move(element));
    }

    /**
    * Removes an element from the repository
    * @param element pointer to the element to remove
    */
    void remove(const T& element) {
        if (!element)
            return;

        collection.erase(std::remove(collection.begin(), collection.end(), element), collection.end());
    }

    /**
    * Significant information about the repository.
    */
    std::string report() const {
        return std::to_string(size()) + " objects";
    }

    /**
    * Number of entries in the repository
    */
    size_t size() const {
        return collection.size();
    }

    /**
     * Finds the first element satisfying the predicate
     * @param predicate predicate
     * @returns repository element
     */
    T findFirst(const Predicate<T>& predicate) {
        auto it = std::find_if(collection.begin(), collection.end(), predicate);
        if (it == collection.end())
            return nullptr;
        return *it;
    }

    /**
    * Finds an element in the repository based on the predicate
    * @param predicate predicate
    * @returns vector of elements from the repository that satisfy the predicate
    */
    std::vector<T> findAll(const Predicate<T>& predicate) {
        std::vector<T> result;
        std::copy_if(collection.begin(), collection.end(), std::back_inserter(result), predicate);
        return result;
    }

    /**
     * @returns All objects in the repository
     */
    const std::vector<T>& getAll() {
        return collection;
    }

    /**
     * Removes all objects from the repository
     */
    void removeAll() {
        collection.clear();
    }
private:
    std::vector<T> collection;
};
