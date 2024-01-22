#include <emias/process_fullstate.hpp>
#include <emias/request.hpp>

#include <iostream>

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
        auto requestAppointments = TJson::parse(R"({"id": "1", "jsonrpc": "2.0", "method": "getAppointmentReceptionsByPatient"})");
        requestAppointments["params"]["omsNumber"] = requestData.omsNumber; 
        requestAppointments["params"]["birthDate"] = requestData.birthDate;
        const auto existingAppointments = NEmias::PostRequest(std::move(requestAppointments));

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
            const TJson::number_unsigned_t availableResourceId)
    {
        TJson requestAvailableResource = TJson::parse(R"({"id": "1", "jsonrpc": "2.0", "method": "getAvailableResourceScheduleInfo"})");
        requestAvailableResource["params"]["omsNumber"] = requestData.omsNumber;
        requestAvailableResource["params"]["birthDate"] = requestData.birthDate;
        requestAvailableResource["params"]["specialityId"] = requestData.specialityId;
        requestAvailableResource["params"]["availableResourceId"] = availableResourceId; 
        if (appointmentId != 0) {
            requestAvailableResource["params"]["appointmentId"] = appointmentId;
        }
        TJson availableResource = NEmias::PostRequest(std::move(requestAvailableResource));

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
                    request[std::to_string(NEmias::OMS_ID)].template get<std::string>(),
                    request[std::to_string(NEmias::BIRTHDATE)].template get<std::string>(),
                    request[std::to_string(NEmias::START_DATE)].template get<std::string>(),
                    request[std::to_string(NEmias::FINISH_DATE)].template get<std::string>(),
                    request[std::to_string(NEmias::SPECIALITY_ID)].template get<TJson::number_unsigned_t>()
                };

                const auto appointmentId = GetExistingAppointment(requestData);
                TJson requestDoctorsInfo = TJson::parse(R"({"id": "1", "jsonrpc": "2.0", "method": "getDoctorsInfo"})");
                requestDoctorsInfo["params"]["omsNumber"] = requestData.omsNumber; 
                requestDoctorsInfo["params"]["birthDate"] = requestData.birthDate;
                requestDoctorsInfo["params"]["specialityId"] = requestData.specialityId;
                if (appointmentId != 0) {
                    requestDoctorsInfo["params"]["appointmentId"] = appointmentId;
                }
                TJson doctorsInfo = NEmias::PostRequest(std::move(requestDoctorsInfo)); 

                for (const auto& item : doctorsInfo["result"]) {
                    if (item["mainDoctor"]["firstName"].template get<std::string>() != requestData.firstName
                        || item["mainDoctor"]["lastName"].template get<std::string>() != requestData.lastName
                        || item["mainDoctor"]["secondName"].template get<std::string>() != requestData.secondName) {
                        continue;
                    }

                    if (InspectAvailableResource(requestData, appointmentId, item["id"].template get<TJson::number_unsigned_t>())) {
                        confirmedRequests.emplace_back(chatId, requestId);
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
