/** @file */

#ifndef EMIAS_CYRILLIC_UNICODE_HPP
#define EMIAS_CYRILLIC_UNICODE_HPP


#include <string>


namespace NEmias::NTools {

    /**
     * \brief Function to convert unicode representation to cyrillic symbols
     * @param inputStr String with raw unicode (\u0430)
     * @return String with unicode symbols replaced with cyrillic ones
     */
    std::string UnicodeToCyrillic(const std::string& inputStr);

} // NEmias


#endif
