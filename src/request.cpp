#include <emias/request.hpp>
#include <emias/tools/cyrillic_unicode.hpp>
#include <cpr/cpr.h>

TJson NEmias::PostRequest(const std::string& requestBody) {
    cpr::Response response = cpr::Post(
        cpr::Url("https://emias.info/api/emc/appointment-eip/v1"),
        cpr::Body(requestBody));
    return TJson::parse(NEmias::NTools::UnicodeToCyrillic(response.text));
}
