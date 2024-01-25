#ifndef EMIAS_REQUEST_HPP
#define EMIAS_REQUEST_HPP


#include <nlohmann/json.hpp>
using TJson = nlohmann::json;


namespace NEmias {

    TJson PostRequest(TJson requestBody);

} // NEmias


#endif

