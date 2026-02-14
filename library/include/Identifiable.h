#pragma once
#include <string>

/**
 * Abstract class whose objects can be uniquely distinguished
 * by a unique identifier
 */
class Identifiable {
public:
    /**
     * Virtual destructor
     */
    virtual ~Identifiable() = default;

    /**
     * @returns Unique object identifier
     */
    virtual std::string getId() const = 0;
};
