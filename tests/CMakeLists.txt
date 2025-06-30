project(tests)

qt_add_executable(XmlParserTest XmlParser_test.cpp)
add_test(NAME XmlParserTest COMMAND XmlParserTest)

target_link_libraries(XmlParserTest PRIVATE Qt::Test StudyGuide_Logic)