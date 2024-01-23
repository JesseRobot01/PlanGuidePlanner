set(SOURCES main.cpp
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
        guide/GuideData.h
        themes/GuidePalette.cpp
        themes/GuidePalette.h
)

set(UI
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

)

set(TRANSLATIONS
        translations/StudyGuide_nl.ts
        translations/StudyGuide_en.ts
)

if (DEFINED WIN32)
    set(WIN_SOURCES
            WindowConsole.cpp
            WindowConsole.h)
endif ()

qt_add_executable(${EXECUTABLE_NAME} ${SOURCES} ${UI} ${WIN_SOURCES})

qt_add_translations(${EXECUTABLE_NAME} TS_FILES ${TRANSLATIONS}
        QM_FILES_OUTPUT_VARIABLE qm_files)

target_include_directories(${EXECUTABLE_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Iterate over each translation file and set QT_RESOURCE_ALIAS
foreach (translation ${qm_files})
    string(REPLACE "${CMAKE_BINARY_DIR}/StudyGuide/" "" translation_name ${translation})
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


target_link_libraries(${EXECUTABLE_NAME} PUBLIC Qt::Core Qt::Gui Qt::Widgets BuildConfig)


#The bundle mess!!!

set(QT_PLUGINS_DIR ${QT6_INSTALL_PREFIX}/${QT6_INSTALL_PLUGINS})
set(QT_LIBS_DIR ${QT6_INSTALL_PREFIX}/${QT6_INSTALL_LIBS})
set(QT_LIBEXECS_DIR ${QT6_INSTALL_PREFIX}/${QT6_INSTALL_LIBEXECS})
set(DIRS ${QT_LIBS_DIR} ${QT_LIBEXECS_DIR} ${CMAKE_LIBRARY_OUTPUT_DIRECTORY} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

if (WIN32)
    set(APPS "\${CMAKE_INSTALL_PREFIX}/${EXECUTABLE_NAME}.exe")

    set(BINARY_DEST_DIR ".")
    set(LIBRARY_DEST_DIR ".")
    set(PLUGIN_DEST_DIR ".")
    set(RESOURCES_DEST_DIR ".")

endif ()

install(TARGETS ${EXECUTABLE_NAME}
        BUNDLE DESTINATION "." COMPONENT Runtime
        LIBRARY DESTINATION ${LIBRARY_DEST_DIR} COMPONENT Runtime
        RUNTIME DESTINATION ${BINARY_DEST_DIR} COMPONENT Runtime
)

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

