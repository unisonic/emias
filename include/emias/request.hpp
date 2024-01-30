/** @file */

#ifndef EMIAS_REQUEST_HPP
#define EMIAS_REQUEST_HPP


#include <nlohmann/json.hpp>

/**
 * Typedef for convenience
 */
using TJson = nlohmann::json;

#include <string>


namespace NEmias {

    /**
     * \brief Function sending post request to https://emias.info
     * @param requestBody Json string with request params
     * @return TJson object with server response
     */ 
    TJson PostRequest(const std::string& requestBody);

} // NEmias


#endif

