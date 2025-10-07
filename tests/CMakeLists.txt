project(tests)

qt_add_executable(XmlParserTest XmlParser_test.cpp)
qt_add_executable(LegacyXmlParsersTest LegacyXmlParsers_test.cpp)

add_test(NAME XmlParserTest COMMAND XmlParserTest)
add_test(NAME LegacyXmlParsersTest COMMAND LegacyXmlParsersTest)


target_link_libraries(XmlParserTest PRIVATE Qt::Test PlanGuidePlanner_Logic)
target_link_libraries(LegacyXmlParsersTest PRIVATE Qt::Test PlanGuidePlanner_Logic)