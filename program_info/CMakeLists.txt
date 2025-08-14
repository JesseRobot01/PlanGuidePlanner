if(ANDROID)
configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/Android/AndroidManifest.xml.in
        ${CMAKE_BINARY_DIR}/Android/AndroidManifest.xml
        @ONLY
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Android/res DESTINATION ${CMAKE_BINARY_DIR}/Android)

endif()