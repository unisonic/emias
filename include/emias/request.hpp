#ifndef EMIAS_REQUEST_HPP
#define EMIAS_REQUEST_HPP


#include <nlohmann/json.hpp>
using TJson = nlohmann::json;

#include <string>


namespace NEmias {

    TJson PostRequest(const std::string& requestBody);

} // NEmias


#endif

