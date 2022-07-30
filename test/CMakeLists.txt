set(TEST_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(TEST_TARGET_NAME "GLaDOS_UnitTest")
set(CATCH_BUILD_STATIC_LIBRARY ON)

add_executable(${TEST_TARGET_NAME})
file(GLOB_RECURSE TEST_SOURCE_FILES "${TEST_SOURCE_DIR}/*.cpp")
target_sources(${TEST_TARGET_NAME}
        PRIVATE
        ${TEST_SOURCE_FILES})
target_link_libraries(${TEST_TARGET_NAME}
        PRIVATE
        Catch2::Catch2WithMain
        ${PROJECT_NAME})
target_include_directories(${TEST_TARGET_NAME} PRIVATE ${LIB_GLADOS_SOURCE_DIR})

include(Catch)
catch_discover_tests(${TEST_TARGET_NAME})
