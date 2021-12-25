add_library(GLaDOS_BenchMarkMain STATIC BenchMarkMain.cpp)
target_link_libraries(GLaDOS_BenchMarkMain PUBLIC benchmark::benchmark)

set(BENCHMARK_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(BENCHMARK_TARGET_NAME "GLaDOS_BenchMark")

add_executable(${BENCHMARK_TARGET_NAME})
file(GLOB_RECURSE BENCHMARK_SOURCE_FILES "${BENCHMARK_SOURCE_DIR}/*.cpp")
target_sources(${BENCHMARK_TARGET_NAME}
        PRIVATE
        ${BENCHMARK_SOURCE_FILES})
target_link_libraries(${BENCHMARK_TARGET_NAME}
        PRIVATE
        GLaDOS_BenchMarkMain
        ${PROJECT_NAME})
target_include_directories(${BENCHMARK_TARGET_NAME} PRIVATE ${LIB_GLADOS_SOURCE_DIR})