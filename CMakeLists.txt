cmake_minimum_required(VERSION 3.23)
project(PlanGuidePlanner)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)

set(STUDYGUIDE_VERSION_MAJOR 0)
set(STUDYGUIDE_VERSION_MINOR 1)
set(STUDYGUIDE_VERSION_PATCH 0)
set(STUDYGUIDE_VERSION ${STUDYGUIDE_VERSION_MAJOR}.${STUDYGUIDE_VERSION_MINOR}.${STUDYGUIDE_VERSION_PATCH})

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets LinguistTools Core5Compat Test Svg)

if (WIN32 OR EMSCRIPTEN)
    set(StudyGuide_FORCE_BUNDLED_LIBS 1) # Hacky fix for windows and emsdk
endif ()

if (NOT StudyGuide_FORCE_BUNDLED_LIBS AND NOT FORCE_BUNDLED_ZLIB)
    find_package(ZLIB QUIET)
endif ()
if (NOT ZLIB_FOUND)
    set(FORCE_BUNDLED_ZLIB TRUE CACHE BOOL "")
    mark_as_advanced(FORCE_BUNDLED_ZLIB)
endif ()

if (NOT StudyGuide_FORCE_BUNDLED_LIBS)
    find_package(QuaZip-Qt6 1.3 QUIET)
endif ()
if (NOT QuaZip-Qt6_FOUND)
    set(QUAZIP_QT_MAJOR_VERSION 6)
    set(FORCE_BUNDLED_QUAZIP 1)
endif ()

option(BUILD_TESTING "Build the testing tree." ON)
option(DEV_BUILD "Development Build" ON)

include(CTest)
if (BUILD_TESTING)
    enable_testing()
endif ()

set(EXECUTABLE_NAME "PlanGuidePlanner")
set(APP_BINARY_NAME "planguideplanner" CACHE STRING "Name of the binary")

set(CMAKE_WIN32_EXECUTABLE ON)

set(QT_ANDROID_BUILD_ALL_ABIS 1)

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

execute_process(
        COMMAND git rev-list --count HEAD
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_COMMIT_NUMBER
        OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
        COMMAND git rev-parse --abbrev-ref HEAD
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_BRANCH
        OUTPUT_STRIP_TRAILING_WHITESPACE
)

message(STATUS "Git tag: ${GIT_TAG}")
message(STATUS "Git commit hash: ${GIT_COMMIT_HASH}")
message(STATUS "Git Branch: ${GIT_BRANCH}")
message(STATUS "Git Branch Commit: ${GIT_COMMIT_NUMBER}")

if (FORCE_BUNDLED_ZLIB)
    message(STATUS "Using bundled zlib")

    set(ZLIB_BUILD_TESTING 0)
    set(ZLIB_INSTALL 0)

    set(ZLIB_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/libraries/zlib)
    set(ZLIB_INCLUDE ${ZLIB_INCLUDE_DIR}/libz.a)
    set(ZCONF_INCLUDE ${ZLIB_INCLUDE_DIR}/zconf.h)

    add_subdirectory(libraries/zlib)


    set(ZLIB_LIBRARY ZLIB::ZLIBSTATIC CACHE STRING "zlib library name")
else ()
    message(STATUS "Using system zlib")
endif ()
if (FORCE_BUNDLED_QUAZIP)
    message(STATUS "Using bundled QuaZip")
    set(BUILD_SHARED_LIBS 0)  # link statically to avoid conflicts.
    set(QUAZIP_INSTALL 0)
    add_subdirectory(libraries/quazip) # zip manipulation library
else ()
    message(STATUS "Using system QuaZip")
endif ()

string(TIMESTAMP TODAY "%Y-%m-%d")
set(STUDYGUIDE_BUILD_DATE "${TODAY}")

if (DEV_BUILD)
    set(STUDYGUIDE_VERSION "${STUDYGUIDE_VERSION}-r${GIT_COMMIT_NUMBER}.${GIT_COMMIT_HASH}")
endif ()

# Cpack stuff
set(CPACK_PACKAGE_NAME "PlanGuidePlanner")
set(CPACK_NSIS_PACKAGE_NAME ${CPACK_PACKAGE_NAME})
set(CPACK_PACKAGE_VENDOR "JesseRobot01")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "PlanGuidePlanner")
set(CPACK_NSIS_MODIFY_PATH "YES")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")

set(CPACK_PACKAGE_EXECUTABLES "..\\\\${EXECUTABLE_NAME}" "PlanGuidePlanner") #The ..\ is because CPack looks in the BIN directory, and not in the root as where the executable is.
set(CPACK_CREATE_DESKTOP_LINKS "..\\\\${EXECUTABLE_NAME}" "PlanGuidePlanner")
set(CPACK_NSIS_MUI_ICON "${CMAKE_SOURCE_DIR}/program_info/StudyGuide.ico")

set(CPACK_PACKAGE_VERSION ${STUDYGUIDE_VERSION})
set(CPACK_PACKAGE_VERSION_MAJOR ${STUDYGUIDE_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${STUDYGUIDE_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${STUDYGUIDE_VERSION_PATCH})

set(CPACK_DEBIAN_PACKAGE_DEPENDS "libqt6core6 (>= ${Qt6Widgets_VERSION}")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libqt6svg6 (>= ${Qt6Widgets_VERSION}")

set(CPACK_PACKAGE_CONTACT "JesseRobot01")

set(CPACK_NSIS_EXTRA_INSTALL_COMMANDS "
  ; Associate .pgd files
  WriteRegStr HKLM  'Software\\\\Classes\\\\.pgd' '' 'planguideplanner.pgd'
  WriteRegStr HKLM  'Software\\\\Classes\\\\planguideplanner.pgd' '' 'Plan Guide Planner File'
  WriteRegStr HKLM  'Software\\\\Classes\\\\planguideplanner.pgd\\\\DefaultIcon' '' '\$INSTDIR\\\\planguideplanner.exe,0'
  WriteRegStr HKLM  'Software\\\\Classes\\\\planguideplanner.pgd\\\\Shell\\\\Open\\\\Command' '' '\\\"$INSTDIR\\\\planguideplanner.exe\\\" \\\"%1\\\"'

  ; Associate .pgm files
  WriteRegStr HKLM  'Software\\\\Classes\\\\.pgm' '' 'planguideplanner.pgm'
  WriteRegStr HKLM  'Software\\\\Classes\\\\planguideplanner.pgm' '' 'Plan Guide Planner Multi File'
  WriteRegStr HKLM  'Software\\\\Classes\\\\planguideplanner.pgm\\\\DefaultIcon' '' '\$INSTDIR\\\\planguideplanner.exe,0'
  WriteRegStr HKLM  'Software\\\\Classes\\\\planguideplanner.pgm\\\\Shell\\\\Open\\\\Command' '' '\\\"$INSTDIR\\\\planguideplanner.exe\\\" \\\"%1\\\"'

  ; Associate .pgx files
  WriteRegStr HKLM  'Software\\\\Classes\\\\.pgx' '' 'planguideplanner.pgx'
  WriteRegStr HKLM  'Software\\\\Classes\\\\planguideplanner.pgx' '' 'Plan Guide Planner XML based File'
  WriteRegStr HKLM  'Software\\\\Classes\\\\planguideplanner.pgx\\\\DefaultIcon' '' '\$INSTDIR\\\\planguideplanner.exe,0'
  WriteRegStr HKLM  'Software\\\\Classes\\\\planguideplanner.pgx\\\\Shell\\\\Open\\\\Command' '' '\\\"$INSTDIR\\\\planguideplanner.exe\\\" \\\"%1\\\"'


  System::Call 'shell32::SHChangeNotify(i 0x08000000, i 0, i 0, i 0)'
")


set(CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "
  DeleteRegKey HKLM  'Software\\\\Classes\\\\planguideplanner.pgd'
  DeleteRegKey HKLM  'Software\\\\Classes\\\\.pgd'

  DeleteRegKey HKLM  'Software\\\\Classes\\\\planguideplanner.pgm'
  DeleteRegKey HKLM  'Software\\\\Classes\\\\.pgm'

  DeleteRegKey HKLM  'Software\\\\Classes\\\\planguideplanner.pgx'
  DeleteRegKey HKLM  'Software\\\\Classes\\\\.pgx'


  System::Call 'shell32::SHChangeNotify(i 0x08000000, i 0, i 0, i 0)'
")


if (UNIX)
    set(CPACK_PACKAGE_NAME "PlanGuidePlanner")
endif ()
include(InstallRequiredSystemLibraries)
include(CPack)

if (LINUX)
    set(BINARY_DEST_DIR "bin")
    set(LIBRARY_DEST_DIR "lib")

    install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/program_info/planguideplanner.desktop" DESTINATION "share/applications")
    install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/program_info/SG-AppIcon.svg" RENAME "planguideplanner.svg" DESTINATION "share/icons/hicolor/scalable/apps")
    install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/program_info/planguideplanner-mime.xml" DESTINATION "share/mime/packages")

    install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/program_info/SG-AppIcon.svg" RENAME "application-x-planguideplanner-pgd.svg" DESTINATION "share/icons/hicolor/scalable/mimetypes")
    install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/program_info/SG-AppIcon.svg" RENAME "application-x-planguideplanner-pgm.svg" DESTINATION "share/icons/hicolor/scalable/mimetypes")
    install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/program_info/SG-AppIcon.svg" RENAME "application-x-planguideplanner-pgx.svg" DESTINATION "share/icons/hicolor/scalable/mimetypes")

endif ()

add_subdirectory(buildconfig)
add_subdirectory(program_info)

if (BUILD_TESTING)
    add_subdirectory(tests)
endif ()

add_subdirectory(PlanGuidePlanner)

