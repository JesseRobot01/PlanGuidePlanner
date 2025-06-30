project(tests)

qt_add_executable(XmlParser_test XmlParser_test.cpp)
add_test(NAME XmlParser_test COMMAND XmlParserTest)

target_link_libraries(XmlParser_test PRIVATE Qt::Test StudyGuide_Logic)