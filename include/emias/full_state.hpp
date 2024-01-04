#ifndef EMIAS_FULL_STATE_HPP
#define EMIAS_EFULL_STATE_HPP

#include <nlohmann/json.hpp>
using TJson = nlohmann::json;


namespace NEmias {

    TJson& GetFullState();

} // NEmias

#endif // EMIAS_FULL_STATE_H
