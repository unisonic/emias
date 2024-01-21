#include <emias/process_fullstate.hpp>
#include <emias/request.hpp>

#include <iostream>

void NEmias::ProcessFullState() {
    for (const auto& [chatId, entries] : NEmias::GFullState.items()) {
        for (const auto& [requestId, entry] : entries.items()) {
            std::string omsNumber = entry[std::to_string(OMS_ID)].template get<std::string>();
            std::string birthDate = entry[std::to_string(BIRTHDATE)].template get<std::string>();

            //  Check existing appointments
            TJson requestAppointments = TJson::parse(R"({"id": "1", "jsonrpc": "2.0", "method": "getAppointmentReceptionsByPatient"})");
            requestAppointments["params"]["omsNumber"] = omsNumber; 
            requestAppointments["params"]["birthDate"] = birthDate;
            TJson existingAppointments = NEmias::PostRequest(std::move(requestAppointments));

            TJson::number_unsigned_t appointmentId = 0;
            TJson::number_unsigned_t specialityId = entry[std::to_string(SPECIALITY_ID)].template get<TJson::number_unsigned_t>();
            for (const auto& appointment : existingAppointments["result"]) {
                const TJson::number_unsigned_t appointmentSpecialityId = appointment["toDoctor"]["specialityId"].template get<TJson::number_unsigned_t>();
                if (specialityId == appointmentSpecialityId || specialityId == 200 && appointmentSpecialityId == 602) {
                    specialityId = appointmentSpecialityId;
                    appointmentId = appointment["id"].template get<TJson::number_unsigned_t>();
                }
            }

            //  Retrieve available appointments
            TJson requestDoctorsInfo = TJson::parse(R"({"id": "1", "jsonrpc": "2.0", "method": "getDoctorsInfo"})");
            requestDoctorsInfo["params"]["omsNumber"] = omsNumber; 
            requestDoctorsInfo["params"]["birthDate"] = birthDate;
            requestDoctorsInfo["params"]["specialityId"] = specialityId;
            if (appointmentId != 0) {
                requestDoctorsInfo["params"]["appointmentId"] = appointmentId;
            }
            TJson doctorsInfo = NEmias::PostRequest(std::move(requestDoctorsInfo)); 

            //  Find suitable appointment
            std::string firstName = entry[std::to_string(FIRST_NAME)].template get<std::string>();
            std::string lastName = entry[std::to_string(LAST_NAME)].template get<std::string>();
            std::string secondName = entry[std::to_string(SECOND_NAME)].template get<std::string>();
            for (const auto& item : doctorsInfo["result"]) {
                if (item["mainDoctor"]["firstName"].template get<std::string>() != firstName
                    || item["mainDoctor"]["lastName"].template get<std::string>() != lastName
                    || item["mainDoctor"]["secondName"].template get<std::string>() != secondName
                ) { continue; }

                TJson::number_unsigned_t availableResourceId = item["id"].template get<TJson::number_unsigned_t>();
                TJson requestAvailableResource = TJson::parse(R"({"id": "1", "jsonrpc": "2.0", "method": "getAvailableResourceScheduleInfo"})");
                requestAvailableResource["params"]["omsNumber"] = omsNumber;
                requestAvailableResource["params"]["birthDate"] = birthDate;
                requestAvailableResource["params"]["specialityId"] = specialityId;
                requestAvailableResource["params"]["availableResourceId"] = availableResourceId; 
                if (appointmentId != 0) {
                    requestAvailableResource["params"]["appointmentId"] = appointmentId;
                }
                TJson availableResource = NEmias::PostRequest(std::move(requestAvailableResource));

                if (availableResource["result"].empty()) {
                    continue;
                }
                
                std::string startDate = entry[std::to_string(START_DATE)].template get<std::string>();
                std::string finishDate = entry[std::to_string(FINISH_DATE)].template get<std::string>();
                for (const auto& slot : availableResource["result"]["scheduleOfDay"]) {
                    std::string slotDate = slot["date"].template get<std::string>();
                    if (startDate <= slotDate && slotDate <= finishDate) { 
                        NEmias::GMainBot.getApi().sendMessage(chatId, "По запросу <b>" + requestId + "</b> доступна запись: <b>" + slotDate + "</b>", false, 0, nullptr, "HTML");
                    }
                }
            }
        }
    }
}
