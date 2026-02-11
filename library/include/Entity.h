#pragma once
#include "Identifiable.h"
#include "Serializable.h"

/**
 * Klasa abstrakcyjna zarządzana przez StorageHandler, której obiekty
 * można utrwalić i jednoznacznie rozróżnić poprzez unikalny identyfikator
 */
class Entity : public Serializable, public Identifiable {};
