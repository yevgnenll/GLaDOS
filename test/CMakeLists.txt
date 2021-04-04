set(CATCH_INCLUDE_DIR "${VENDOR_DIR}/catch")
add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE ${CATCH_INCLUDE_DIR})

add_library(CatchMain STATIC TestMain.cpp)
target_link_libraries(CatchMain PUBLIC Catch)

set(CMAKE_CXX_STANDARD 17)
set(TEST_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(TEST_TARGET_NAME "UnitTest")

add_executable(${TEST_TARGET_NAME})
file(GLOB_RECURSE TEST_SOURCE_FILES "${TEST_SOURCE_DIR}/*.cpp")
target_sources(${TEST_TARGET_NAME}
        PRIVATE
        ${TEST_SOURCE_FILES})
target_link_libraries(${TEST_TARGET_NAME}
        PRIVATE
        CatchMain
        ${PROJECT_NAME}
        project_warnings
        project_options)
target_include_directories(${TEST_TARGET_NAME} PRIVATE ${LIB_GLADOS_SOURCE_DIR})

include(ParseAndAddCatchTests)
ParseAndAddCatchTests(${TEST_TARGET_NAME})

