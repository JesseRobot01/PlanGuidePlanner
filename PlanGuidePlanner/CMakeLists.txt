set(SOURCES
        Application.cpp
        Application.h
        XmlParser.cpp
        XmlParser.h
        ui/MainWindow.cpp
        ui/MainWindow.h
        ui/PreferenceWindow.cpp
        ui/PreferenceWindow.h
        ui/guide/Guide.cpp
        ui/guide/Guide.h
        ui/guide/Index.cpp
        ui/guide/Index.h
        ui/guide/Goal.cpp
        ui/guide/Goal.h
        ui/guide/Test.cpp
        ui/guide/Test.h
        ui/guide/Report.cpp
        ui/guide/Report.h
        ui/dialogs/LoadGuide.cpp
        ui/dialogs/LoadGuide.h
        guide/OldGuideData.h
        themes/GuidePalette.cpp
        themes/GuidePalette.h
        guide/OldGuideData.cpp
        LegacyXmlParsers.cpp
        LegacyXmlParsers.h
        themes/BlueTheme.h
        themes/GreenTheme.h
        themes/RedTheme.h
        themes/OrangeTheme.h
        themes/ContrastTheme.h
        ui/prototypeGui/newguideprototype.cpp
        ui/prototypeGui/newguideprototype.h
        guide/NewGuideData.cpp
)

set(UI
        ui/prototypeGui/newGuidePrototype.ui
        ui/MainWindow.ui
        ui/PreferenceWindow.ui
        ui/guide/Guide.ui
        ui/guide/Index.ui
        ui/guide/Goal.ui
        ui/guide/Test.ui
        ui/guide/Report.ui
        ui/dialogs/LoadGuide.ui
        ui/dialogs/RestartDialog.cpp
        ui/dialogs/RestartDialog.h
        ui/dialogs/RestartDialog.ui
        ui/AboutWindow.cpp
        ui/AboutWindow.h
        ui/About.ui
        ui/StartScreen.cpp
        ui/StartScreen.h
        ui/StartScreen.ui

)

set(CREATOR_SOURCES
        creator/Creator.h
        creator/Creator.cpp
        creator/Creator.ui)

set(TRANSLATIONS
        translations/PlanGuidePlanner_nl.ts
        translations/PlanGuidePlanner_en.ts
)

set(LOGO_FILES
        ../program_info/PGP-AppIcon.svg
        ../program_info/PGP-full-DarkMode.svg
        ../program_info/PGP-full-LightMode.svg
../program_info/PGP-Logo.svg)

if (DEFINED WIN32)
    set(WIN_SOURCES
            WindowConsole.cpp
            WindowConsole.h)
endif ()

qt_add_library(PlanGuidePlanner_Logic STATIC ${SOURCES} ${UI} ${WIN_SOURCES} ${CREATOR_SOURCES})
target_link_libraries(PlanGuidePlanner_Logic Qt::Core Qt::Gui Qt::Widgets BuildConfig QuaZip::QuaZip Qt::Svg Qt6::Core5Compat ${ZLIB_LIBRARY})
if (ANDROID)
    target_link_libraries(PlanGuidePlanner_Logic Qt::CorePrivate)
endif ()


qt_add_executable(${EXECUTABLE_NAME} main.cpp ../program_info/planguideplanner.rc)
target_link_libraries(${EXECUTABLE_NAME} PRIVATE PlanGuidePlanner_Logic)

target_include_directories(PlanGuidePlanner_Logic PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if (DEFINED APP_BINARY_NAME)
    set_target_properties(${EXECUTABLE_NAME} PROPERTIES OUTPUT_NAME "${APP_BINARY_NAME}")
endif ()

if (ANDROID)
    set(android_source_dir ${CMAKE_BINARY_DIR}/Android)
    message(${android_source_dir})
    set_target_properties(${EXECUTABLE_NAME} PROPERTIES QT_ANDROID_PACKAGE_SOURCE_DIR ${android_source_dir})
endif ()

qt_add_translations(${EXECUTABLE_NAME} TS_FILES ${TRANSLATIONS}
        QM_FILES_OUTPUT_VARIABLE qm_files)

# Iterate over each translation file and set QT_RESOURCE_ALIAS
foreach (translation ${qm_files})
    string(REPLACE "${CMAKE_BINARY_DIR}/PlanGuidePlanner/" "" translation_name ${translation})
    string(REPLACE ".qm" \; translation_name ${translation_name})

    # Set QT_RESOURCE_ALIAS for the qm file
    set_source_files_properties(
            ${translation}
            PROPERTIES
            QT_RESOURCE_ALIAS "${translation_name}"
    )
endforeach ()


qt_add_resources(${EXECUTABLE_NAME} translations
        PREFIX "/translations"
        FILES ${qm_files}
)

foreach (logo ${LOGO_FILES})
    string(REPLACE "../program_info/" "" logo_name ${logo})

    # Set QT_RESOURCE_ALIAS for the qm file
    set_source_files_properties(
            ${logo}
            PROPERTIES
            QT_RESOURCE_ALIAS "${logo_name}"
    )
endforeach ()

qt_add_resources(${EXECUTABLE_NAME} logos
        PREFIX "/logos"
        FILES ${LOGO_FILES}
)

#The bundle mess!!!

set(QT_PLUGINS_DIR ${QT6_INSTALL_PREFIX}/${QT6_INSTALL_PLUGINS})
set(QT_LIBS_DIR ${QT6_INSTALL_PREFIX}/${QT6_INSTALL_LIBS})
set(QT_LIBEXECS_DIR ${QT6_INSTALL_PREFIX}/${QT6_INSTALL_LIBEXECS})
set(DIRS ${QT_LIBS_DIR} ${QT_LIBEXECS_DIR} ${CMAKE_LIBRARY_OUTPUT_DIRECTORY} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ${ZLIB_INCLUDE_DIR})

if (WIN32)
    set(APPS "\${CMAKE_INSTALL_PREFIX}/${EXECUTABLE_NAME}.exe")
endif ()
if (WIN32 OR EMSCRIPTEN)
    set(BINARY_DEST_DIR ".")
    set(LIBRARY_DEST_DIR ".")
    set(PLUGIN_DEST_DIR ".")
    set(RESOURCES_DEST_DIR ".")
endif ()

if (NOT ANDROID)
    install(TARGETS ${EXECUTABLE_NAME}
            BUNDLE DESTINATION "." COMPONENT Runtime
            LIBRARY DESTINATION ${LIBRARY_DEST_DIR} COMPONENT Runtime
            RUNTIME DESTINATION ${BINARY_DEST_DIR} COMPONENT Runtime
    )
endif ()

if (WIN32)
    #Example.xml
    install(
            FILES Example.xml
            DESTINATION .
            COMPONENT Runtime
    )
    # Platform
    install(
            DIRECTORY "${QT_PLUGINS_DIR}/platforms"
            CONFIGURATIONS Debug RelWithDebInfo ""
            DESTINATION ${PLUGIN_DEST_DIR}
            COMPONENT Runtime
            REGEX "minimal|linuxfb|offscreen" EXCLUDE
    )
    install(
            DIRECTORY "${QT_PLUGINS_DIR}/platforms"
            CONFIGURATIONS Release MinSizeRel
            DESTINATION ${PLUGIN_DEST_DIR}
            COMPONENT Runtime
            REGEX "minimal|linuxfb|offscreen" EXCLUDE
            REGEX "[^2]d\\." EXCLUDE
            REGEX "_debug\\." EXCLUDE
            REGEX "\\.dSYM" EXCLUDE
    )

    # Style plugins
    if (EXISTS "${QT_PLUGINS_DIR}/styles")
        install(
                DIRECTORY "${QT_PLUGINS_DIR}/styles"
                CONFIGURATIONS Debug RelWithDebInfo ""
                DESTINATION ${PLUGIN_DEST_DIR}
                COMPONENT Runtime
        )
        install(
                DIRECTORY "${QT_PLUGINS_DIR}/styles"
                CONFIGURATIONS Release MinSizeRel
                DESTINATION ${PLUGIN_DEST_DIR}
                COMPONENT Runtime
                REGEX "d\\." EXCLUDE
                REGEX "_debug\\." EXCLUDE
                REGEX "\\.dSYM" EXCLUDE
        )
    endif ()

    install(CODE "
include(BundleUtilities)
file(GLOB_RECURSE QTPLUGINS \"${CMAKE_INSTALL_PREFIX}/${PLUGIN_DEST_DIR}/*${CMAKE_SHARED_LIBRARY_SUFFIX}\")
fixup_bundle(\"${APPS}\" \"${QTPLUGINS}\" \"${DIRS}\")"
            COMPONENT Runtime
    )
endif ()

if (EMSCRIPTEN)
    #The glob aint globbing when cmake does not rerun after first build.
    #file(GLOB HTML_FILES
    #        ${CMAKE_CURRENT_BINARY_DIR}/*.html
    #)
    #file(GLOB JS_FILES
    #        ${CMAKE_CURRENT_BINARY_DIR}/*.js
    #)
    #file(GLOB WASM_FILES
    #        ${CMAKE_CURRENT_BINARY_DIR}/*.wasm
    #)
    #file(GLOB SVG_FILES
    #        ${CMAKE_CURRENT_BINARY_DIR}/*.svg
    #)
    #

    install(
            FILES ${CMAKE_CURRENT_BINARY_DIR}/qtloader.js ${CMAKE_CURRENT_BINARY_DIR}/planguideplanner.js ${CMAKE_CURRENT_BINARY_DIR}/planguideplanner.worker.js
            DESTINATION .
            COMPONENT Runtime
    )

    install(
            FILES ${CMAKE_CURRENT_BINARY_DIR}/planguideplanner.wasm
            DESTINATION .
            COMPONENT Runtime
    )

    install(
            FILES ${CMAKE_CURRENT_BINARY_DIR}/qtlogo.svg
            DESTINATION .
            COMPONENT Runtime
    )

    #Exclude the main HTML
    #  list(REMOVE_ITEM HTML_FILES "${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE_NAME}.html")

    install(
            FILES "${CMAKE_CURRENT_BINARY_DIR}/${APP_BINARY_NAME}.html"
            RENAME "index.html"
            DESTINATION .
            COMPONENT Runtime
    )
    #  install(
    #          FILES ${HTML_FILES} ${JS_FILES} ${WASM_FILES} ${SVG_FILES}
    #          DESTINATION .
    #          COMPONENT Runtime
    #  )
endif ()

if (ANDROID)
    file(GLOB APK_FILES
            "${CMAKE_CURRENT_BINARY_DIR}/android-build/*.apk"
    )
    install(
            FILES ${CMAKE_CURRENT_BINARY_DIR}/android-build/${EXECUTABLE_NAME}.apk
            DESTINATION .
    )
endif ()
