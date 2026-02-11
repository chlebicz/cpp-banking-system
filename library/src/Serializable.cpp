#include "Serializable.h"

void to_json(nlohmann::json& j, const Serializable& s) {
    j = s.toJSON();
}

void to_json(nlohmann::json& j, const std::shared_ptr<Serializable>& s) {
    to_json(j, *s);
}
