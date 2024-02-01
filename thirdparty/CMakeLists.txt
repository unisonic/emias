include(FetchContent)
set(FETCHCONTENT_QUIET OFF)

FetchContent_Declare (
    tgbot 
    GIT_REPOSITORY https://github.com/unisonic/tgbot-cpp.git
    GIT_TAG        e4de8d648f38235ae90722af47bc82cd1ab73463 # 1.7.4
)
if (NOT cpr_POPULATED)
    FetchContent_Populate(tgbot)
    if (BUILD_SHARED_LIBS)
        add_subdirectory(${tgbot_SOURCE_DIR} ${tgbot_BINARY_DIR})
    else()
        add_subdirectory(${tgbot_SOURCE_DIR} ${tgbot_BINARY_DIR} EXCLUDE_FROM_ALL)
    endif()
endif()

FetchContent_Declare (
    curlforpeople
    GIT_REPOSITORY https://github.com/libcpr/cpr.git
    GIT_TAG        3b15fa82ea74739b574d705fea44959b58142eb8 # 1.10.5
)
if (NOT cpr_POPULATED)
    FetchContent_Populate(curlforpeople)
    option(CPR_USE_SYSTEM_CURL "If enabled we will use the curl lib already installed on this system." ON)
    if (BUILD_SHARED_LIBS)
        add_subdirectory(${curlforpeople_SOURCE_DIR} ${curlforpeople_BINARY_DIR})
    else()
        add_subdirectory(${curlforpeople_SOURCE_DIR} ${curlforpeople_BINARY_DIR} EXCLUDE_FROM_ALL)
    endif()
endif()

FetchContent_Declare (
    nlohmannjson
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG        9cca280a4d0ccf0c08f47a99aa71d1b0e52f8d03 # 3.11.3
)
if (NOT nlohmannjson_POPULATED)
    FetchContent_Populate(nlohmannjson)
    option(JSON_MultipleHeaders "Use non-amalgamated version of the library." OFF)
    add_subdirectory(${nlohmannjson_SOURCE_DIR} ${nlohmannjson_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()
