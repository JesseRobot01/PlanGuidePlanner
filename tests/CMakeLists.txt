project(tests)

qt_add_executable(XmlParserTest XmlParser_test.cpp)
qt_add_executable(CollectDataTest CollectData_test.cpp)

add_test(NAME XmlParserTest COMMAND XmlParserTest)
add_test(NAME CollectDataTest COMMAND CollectDataTest)


target_link_libraries(XmlParserTest PRIVATE Qt::Test PlanGuidePlanner_Logic)
target_link_libraries(CollectDataTest PRIVATE Qt::Test PlanGuidePlanner_Logic)