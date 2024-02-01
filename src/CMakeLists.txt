set(SRCS "main.cpp"
         "request.cpp"
         "global_instance.cpp"
         "process_fullstate.cpp"
         "update_fullstate.cpp"
         "tools/cyrillic_unicode.cpp")

set(HDRS "${CMAKE_SOURCE_DIR}/include/emias/request.hpp"
         "${CMAKE_SOURCE_DIR}/include/emias/global_instance.hpp"
         "${CMAKE_SOURCE_DIR}/include/emias/process_fullstate.hpp"
         "${CMAKE_SOURCE_DIR}/include/emias/update_fullstate.hpp"
         "${CMAKE_SOURCE_DIR}/include/emias/tools/timer.hpp"
         "${CMAKE_SOURCE_DIR}/include/emias/tools/cyrillic_unicode.hpp")

find_package(Boost 1.84 REQUIRED)

add_executable(${PROJECT_NAME} ${SRCS} ${HDRS})
target_include_directories(${PROJECT_NAME} PRIVATE BEFORE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} PRIVATE TgBot PRIVATE cpr::cpr PRIVATE nlohmann_json::nlohmann_json PRIVATE Boost::boost)

file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/full_state.json "{}")
