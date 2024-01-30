/** @file */

#include <emias/tools/cyrillic_unicode.hpp>
#include <unordered_map>

namespace {

    std::unordered_map<std::string, std::string> GCyrillicTable = {
        { R"(\u0410)", "А" }, { R"(\u0411)", "Б" }, { R"(\u0412)", "В" }, { R"(\u0413)", "Г" },
        { R"(\u0414)", "Д" }, { R"(\u0415)", "Е" }, { R"(\u0416)", "Ж" }, { R"(\u0417)", "З" },
        { R"(\u0418)", "И" }, { R"(\u0419)", "Й" }, { R"(\u041a)", "К" }, { R"(\u041b)", "Л" },
        { R"(\u041c)", "М" }, { R"(\u041d)", "Н" }, { R"(\u041e)", "О" }, { R"(\u041f)", "П" },
        { R"(\u0420)", "Р" }, { R"(\u0421)", "С" }, { R"(\u0422)", "Т" }, { R"(\u0423)", "У" }, 
        { R"(\u0424)", "Ф" }, { R"(\u0425)", "Х" }, { R"(\u0426)", "Ц" }, { R"(\u0427)", "Ч" },
        { R"(\u0428)", "Ш" }, { R"(\u0429)", "Щ" }, { R"(\u042a)", "Ъ" }, { R"(\u042b)", "Ы" },
        { R"(\u042c)", "Ь" }, { R"(\u042d)", "Э" }, { R"(\u042e)", "Ю" }, { R"(\u042f)", "Я" },
        { R"(\u0430)", "а" }, { R"(\u0431)", "б" }, { R"(\u0432)", "в" }, { R"(\u0433)", "г" },
        { R"(\u0434)", "д" }, { R"(\u0435)", "е" }, { R"(\u0436)", "ж" }, { R"(\u0437)", "з" },
        { R"(\u0438)", "и" }, { R"(\u0439)", "й" }, { R"(\u043a)", "к" }, { R"(\u043b)", "л" },
        { R"(\u043c)", "м" }, { R"(\u043d)", "н" }, { R"(\u043e)", "о" }, { R"(\u043f)", "п" },
        { R"(\u0440)", "р" }, { R"(\u0441)", "с" }, { R"(\u0442)", "т" }, { R"(\u0443)", "у" }, 
        { R"(\u0444)", "ф" }, { R"(\u0445)", "х" }, { R"(\u0446)", "ц" }, { R"(\u0447)", "ч" },
        { R"(\u0448)", "ш" }, { R"(\u0449)", "щ" }, { R"(\u044a)", "ъ" }, { R"(\u044b)", "ы" },
        { R"(\u044c)", "ь" }, { R"(\u044d)", "э" }, { R"(\u044e)", "ю" }, { R"(\u044f)", "я" },
        { R"(\u0401)", "Ё" }, { R"(\u0451)", "ё" }
    };

}

std::string NEmias::NTools::UnicodeToCyrillic(const std::string& inputStr) {
    std::string outStr;
    for (unsigned i = 0u; i < inputStr.size(); ++i) {
        auto iter = GCyrillicTable.find(inputStr.substr(i, 6u));
        if (iter != GCyrillicTable.end()) {
            outStr += iter->second;
            i += 5u;
        } else {
            outStr += inputStr[i];
        }
    }
    return outStr;
}
