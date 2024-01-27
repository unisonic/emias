#include <emias/process_fullstate.hpp>
#include <emias/request.hpp>

#include <iostream>
#include <array>
#include <boost/format.hpp>

namespace {

    struct TRequestData {
        std::string chatId;
        std::string requestId;

        std::string firstName;
        std::string lastName;
        std::string secondName;

        std::string omsNumber;
        std::string birthDate;

        std::string startDate;
        std::string finishDate;

        TJson::number_unsigned_t specialityId;
    };

    TJson::number_unsigned_t GetExistingAppointment(const TRequestData& requestData) {
        const TJson existingAppointments = NEmias::PostRequest(boost::str(boost::format(
            R"({"id": "1", "jsonrpc": "2.0", "method": "getAppointmentReceptionsByPatient", "params":{"omsNumber":"%1%","birthDate":"%2%"}})")
            % requestData.omsNumber % requestData.birthDate));

        TJson::number_unsigned_t appointmentId = 0;
        for (const auto& appointment : existingAppointments["result"]) {
            const auto appointmentSpecialityId = appointment["toDoctor"]["specialityId"].template get<TJson::number_unsigned_t>();
            if (requestData.specialityId == appointmentSpecialityId || requestData.specialityId == 200 && (appointmentSpecialityId == 602 || appointmentSpecialityId == 69)) {
                appointmentId = appointment["id"].template get<TJson::number_unsigned_t>();
            }
        }
        
        return appointmentId;
    }

    bool InspectAvailableResource(
            const TRequestData& requestData,
            const TJson::number_unsigned_t appointmentId,
            const TJson::number_unsigned_t complexResourceId,
            const TJson::number_unsigned_t availableResourceId)
    {
        
        TJson availableResource = TJson::object();
        if (appointmentId == 0) {
            availableResource = NEmias::PostRequest(boost::str(boost::format(
                R"({"id": "1", "jsonrpc": "2.0", "method": "getAvailableResourceScheduleInfo",
                    "params": {"omsNumber":"%1%", "birthDate": "%2%", "specialityId": %3%, "availableResourceId": %4%, "complexResourceId": %5%}})")
                % requestData.omsNumber % requestData.birthDate % requestData.specialityId % availableResourceId % complexResourceId));
        } else {
            availableResource = NEmias::PostRequest(boost::str(boost::format(
                R"({"id": "1", "jsonrpc": "2.0", "method": "getAvailableResourceScheduleInfo",
                    "params": {"omsNumber":"%1%", "birthDate": "%2%", "specialityId": %3%, "availableResourceId": %4%, "complexResourceId": %5%, "appointmentId": %6%}})")
                % requestData.omsNumber % requestData.birthDate % requestData.specialityId % availableResourceId % complexResourceId % appointmentId));
        }

        if (availableResource["result"].empty()) {
            return false;
        }
        
        bool appointmentFound = false;
        for (const auto& slot : availableResource["result"]["scheduleOfDay"]) {
            std::string slotDate = slot["date"].template get<std::string>();
            if (requestData.startDate <= slotDate && slotDate <= requestData.finishDate) { 
                NEmias::GMainBot.getApi().sendMessage(requestData.chatId, "По запросу <b>" + requestData.requestId + "</b> доступна запись на дату <b>" + slotDate + "</b>", false, 0, nullptr, "HTML");
                appointmentFound = true;
            }
        }

        return appointmentFound;
    }

    std::vector<std::pair<std::string, std::string>> ProcessRequests() {
        std::vector<std::pair<std::string, std::string>> confirmedRequests;

        for (const auto& [chatId, requests] : NEmias::GFullState.items()) {
            for (const auto& [requestId, request] : requests.items()) {
                const TRequestData requestData {
                    chatId,
                    requestId,
                    request[std::to_string(NEmias::FIRST_NAME)].template get<std::string>(),
                    request[std::to_string(NEmias::LAST_NAME)].template get<std::string>(),
                    request[std::to_string(NEmias::SECOND_NAME)].template get<std::string>(),
                    request[std::to_string(NEmias::OMS_NUMBER)].template get<std::string>(),
                    request[std::to_string(NEmias::BIRTHDATE)].template get<std::string>(),
                    request[std::to_string(NEmias::START_DATE)].template get<std::string>(),
                    request[std::to_string(NEmias::FINISH_DATE)].template get<std::string>(),
                    request[std::to_string(NEmias::SPECIALITY_ID)].template get<TJson::number_unsigned_t>()
                };

                const auto appointmentId = GetExistingAppointment(requestData);
                TJson doctorsInfo = TJson::object();
                if (appointmentId == 0) {
                    doctorsInfo = NEmias::PostRequest(boost::str(boost::format(
                        R"({"id": "1", "jsonrpc": "2.0", "method": "getDoctorsInfo", "params": {"omsNumber":"%1%", "birthDate": "%2%", "specialityId": %3%}})")
                        % requestData.omsNumber % requestData.birthDate % requestData.specialityId));
                } else {
                    doctorsInfo = NEmias::PostRequest(boost::str(boost::format(
                        R"({"id": "1", "jsonrpc": "2.0", "method": "getDoctorsInfo", "params": {"omsNumber":"%1%", "birthDate": "%2%", "specialityId": %3%, "appointmentId": %4%}})")
                        % requestData.omsNumber % requestData.birthDate % requestData.specialityId % appointmentId));
                }

                for (const auto& item : doctorsInfo["result"]) {
                    if (item["mainDoctor"]["firstName"].template get<std::string>() != requestData.firstName
                        || item["mainDoctor"]["lastName"].template get<std::string>() != requestData.lastName
                        || item["mainDoctor"]["secondName"].template get<std::string>() != requestData.secondName) {
                        continue;
                    }

                    for (const auto& complexResource : item["complexResource"]) {
                        if (InspectAvailableResource(requestData, appointmentId, complexResource["id"].template get<TJson::number_unsigned_t>(), item["id"].template get<TJson::number_unsigned_t>())) {
                            confirmedRequests.emplace_back(chatId, requestId);
                        }
                    }
                }
            }
        }

        return confirmedRequests;
    }

}

void NEmias::ProcessFullState() {
    const auto confirmedRequests = ProcessRequests();
    for (const auto& [chatId, requestId] : confirmedRequests) {
        NEmias::GFullState[chatId].erase(requestId);
    }
}
