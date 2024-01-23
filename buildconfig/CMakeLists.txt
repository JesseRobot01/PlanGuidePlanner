configure_file("${CMAKE_CURRENT_SOURCE_DIR}/Config.cpp.in" "${CMAKE_CURRENT_BINARY_DIR}/Config.cpp")

add_library(BuildConfig STATIC
        Config.h
        ${CMAKE_CURRENT_BINARY_DIR}/Config.cpp
)

target_link_libraries(BuildConfig Qt${QT_VERSION_MAJOR}::Core)
target_include_directories(BuildConfig PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")