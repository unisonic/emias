cmake_minimum_required(VERSION 3.27.9)
project(emias VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)

include(GNUInstallDirs)

add_subdirectory(thirdparty)
add_subdirectory(src)

if (EMIAS_BUILD_DOCS)
    add_subdirectory(docs)
endif()
