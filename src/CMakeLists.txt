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

add_executable(${PROJECT_NAME} ${SRCS} ${HDRS})
if (NOT WIN32 AND BUILD_SHARED_LIBS)
    file(RELATIVE_PATH relDir1
            ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}
            ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR})
    file(RELATIVE_PATH relDir2
            ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}
            ${CMAKE_INSTALL_PREFIX}/lib)
    set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_RPATH "$ORIGIN:$ORIGIN/${relDir1}:$ORIGIN/${relDir2}")
endif()

find_package(Boost 1.84 REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE BEFORE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} PRIVATE TgBot PRIVATE cpr::cpr PRIVATE nlohmann_json::nlohmann_json PRIVATE Boost::boost)

file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/full_state.json "{}")
