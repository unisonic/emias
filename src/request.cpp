#include <emias/request.hpp>
#include <emias/tools/cyrillic_unicode.hpp>
#include <cpr/cpr.h>

TJson NEmias::PostRequest(TJson requestBody) {
    cpr::Response response = cpr::Post(
        cpr::Url("https://emias.info/api/emc/appointment-eip/v1"),
        cpr::Body(requestBody.dump()));
    std::string decoded = NEmias::NTools::UnicodeToCyrillic(response.text);
    return TJson::parse(std::move(decoded));
}
