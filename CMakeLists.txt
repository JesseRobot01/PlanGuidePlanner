cmake_minimum_required(VERSION 3.23)
project(StudyGuide)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)

set(version 1.0.0)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets LinguistTools)

set(EXECUTABLE_NAME "StudyGuide")

set(CMAKE_WIN32_EXECUTABLE ON)

include(InstallRequiredSystemLibraries)
include(CPack)

#Get Git commit and tag
execute_process(
        COMMAND git describe
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_TAG
        OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
        COMMAND git log -1 --format=%h
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_COMMIT_HASH
        OUTPUT_STRIP_TRAILING_WHITESPACE
)

message("Git tag: ${GIT_TAG}")

message("Git commit hash: ${GIT_COMMIT_HASH}")

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    set(version "${version}-${GIT_COMMIT_HASH}")
endif ()

add_subdirectory(StudyGuide)

