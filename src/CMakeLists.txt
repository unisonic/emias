set(SRCS "main.cpp"
         "global_instance.cpp"
         "tools/cyrillic_unicode.cpp")
set(HDRS "${CMAKE_SOURCE_DIR}/include/emias/global_instance.hpp"
         "${CMAKE_SOURCE_DIR}/include/emias/tools/cyrillic_unicode.hpp")

add_executable(${PROJECT_NAME} ${SRCS} ${HDRS})
target_link_libraries(${PROJECT_NAME} PRIVATE TgBot PRIVATE cpr::cpr PRIVATE nlohmann_json::nlohmann_json)
target_include_directories(${PROJECT_NAME} PRIVATE BEFORE ${CMAKE_SOURCE_DIR}/include)
