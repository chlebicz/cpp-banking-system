#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include <functional>
#include "Serializable.h"

/**
 * Predykat dla danego typu
 */
template<typename T>
using Predicate = std::function<bool (const T&)>;

/**
 * Repozytorium - składowanie danych w pamięci
 * @tparam T Typ zarządzany przez repozytorium
 */
template<typename T>
class Repository {
public:
    /**
    * @param id Identyfikator elementu w repozytorium
    * @returns wskaznik do elementu
    */
    T get(int id) {
        if (id >= collection.size())
            return nullptr;
        return collection[id];
    }

    /**
    * Dodaje element do repozytorium
    * @param element wskaznik do elementu do dodania
    */
    void add(T element) {
        if (!element)
            return;
        collection.push_back(std::move(element));
    }

    /**
    * Usuwa element z repozytorium
    * @param element wskaznik do elementu do usuniecia
    */
    void remove(const T& element) {
        if (!element)
            return;

        collection.erase(std::remove(collection.begin(), collection.end(), element), collection.end());
    }

    /**
    * Istotne informacje o repozytorium.
    */
    std::string report() const {
        return std::to_string(size()) + " obiektow";
    }

    /**
    * Liczba wpisow w repozytorium
    */
    size_t size() const {
        return collection.size();
    }

    /**
     * Znajduje pierwszy element spełniający predykat
     * @param predicate predykat
     * @returns element repozytorium
     */
    T findFirst(const Predicate<T>& predicate) {
        auto it = std::find_if(collection.begin(), collection.end(), predicate);
        if (it == collection.end())
            return nullptr;
        return *it;
    }

    /**
    * Znajduje element w repozytorium na podstawie predykatu
    * @param predicate predykat
    * @returns wektor elementow z repozytorium, ktore spelniaja predykat
    */
    std::vector<T> findAll(const Predicate<T>& predicate) {
        std::vector<T> result;
        std::copy_if(collection.begin(), collection.end(), std::back_inserter(result), predicate);
        return result;
    }

    /**
     * @returns Wszystkie obiekty w repozytorium
     */
    const std::vector<T>& getAll() {
        return collection;
    }

    /**
     * Usuwa wszystkie obiekty z repozytorium
     */
    void removeAll() {
        collection.clear();
    }
private:
    std::vector<T> collection;
};
