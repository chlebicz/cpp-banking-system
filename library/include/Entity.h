#pragma once
#include "Identifiable.h"
#include "Serializable.h"

/**
 * Abstract class managed by StorageHandler, whose objects
 * can be persisted and uniquely distinguished by a unique identifier
 */
class Entity : public Serializable, public Identifiable {};
