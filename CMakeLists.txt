cmake_minimum_required(VERSION 3.16)
message(STATUS "Current CMake Version ${CMAKE_VERSION}")
set(PROJECT_VERSION_MAJOR 1)
set(PROJECT_VERSION_MINOR 0)
set(PROJECT_VERSION_PATCH 0)
set(PROJECT_VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})
project(GLaDOS LANGUAGES CXX VERSION ${PROJECT_VERSION})
message(STATUS "Build ${PROJECT_NAME} Version ${CMAKE_PROJECT_VERSION} with [${CMAKE_BUILD_TYPE}] build")

set(CMAKE_CXX_STANDARD 17)
set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")
set(RESOURCE_DIR "${CMAKE_SOURCE_DIR}/resource")
set(SHADER_PREFIX_DIR "${CMAKE_SOURCE_DIR}/shader")
if(APPLE)
  set(SHADER_DIR "${SHADER_PREFIX_DIR}/mlsl")
elseif(UNIX AND NOT APPLE) # LINUX
  set(SHADER_DIR "${SHADER_PREFIX_DIR}/glsl")
elseif(WIN32)
  set(SHADER_DIR "${SHADER_PREFIX_DIR}/hlsl")
endif()
set(SCRIPT_DIR "${CMAKE_SOURCE_DIR}/script")
set(SAMPLE_DIR "${CMAKE_SOURCE_DIR}/sample")
set(VENDOR_DIR "${CMAKE_SOURCE_DIR}/vendor")
set(STB_LIB_DIR "${VENDOR_DIR}/stb")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
  set(MEMORY_TRACE_ALLOCATION 0)
  set(MEMORY_DEBUG_PRINT 0)
else()
  set(MEMORY_TRACE_ALLOCATION 1)
  set(MEMORY_DEBUG_PRINT 1)
endif()
set(LIB_GLADOS_SOURCE_DIR "${SOURCE_DIR}/LibGLaDOS")
set(LIB_GUI_SOURCE_DIR "${SOURCE_DIR}/LibGUI")

# Link this 'library' to set the c++ standard / compile-time options requested
add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_17)

# Link this 'library' to use the warnings specified in CompilerWarnings.cmake
add_library(project_warnings INTERFACE)

# standard compiler warnings
include(CompilerWarnings)
set_project_warnings(project_warnings)

# Static Analyzer settings (cppcheck, clang-tidy, iwyu)
include(StaticAnalyzers)

find_program(CCACHE ccache)
if(CCACHE)
  set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE})
else()
  message(STATUS "ccache not found cannot use")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(GLADOS_ENABLE_TESTING "Enable testing of the GLaDOS." ON)
option(GLADOS_ENABLE_BENCHMARK "Enable benchmarking of the GLaDOS." ON)
option(GLADOS_ENABLE_SAMPLE "Enable sample build." ON)

if (GLADOS_ENABLE_TESTING)
  set(CATCH_BUILD_TESTING OFF CACHE BOOL "disable Catch2 self test")
  cmake_policy(SET CMP0110 NEW)
  add_subdirectory(Catch2)
  enable_testing()
  add_subdirectory(test)
endif ()

if (GLADOS_ENABLE_BENCHMARK)
  set(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE BOOL "disable google test")
  set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "disable google benchmark test")
  add_subdirectory(benchmark)
  add_subdirectory(bench)
endif ()

if(APPLE)
  find_library(COCOA_LIBRARY Cocoa REQUIRED)
  find_library(QUARTZCORE_LIBRARY QuartzCore REQUIRED)
  find_library(METAL_LIBRARY Metal REQUIRED)
  find_package(Threads)
  set(LINK_LIBRARIES ${COCOA_LIBRARY} ${QUARTZCORE_LIBRARY} ${METAL_LIBRARY})
  set(INCLUDE_LIBRARIES ${STB_LIB_DIR})
elseif(UNIX AND NOT APPLE) # LINUX
  find_library(X11_LIBRARY X11 REQUIRED)
  find_package(Threads)
  set(LINK_LIBRARIES ${X11_LIBRARY})
  set(INCLUDE_LIBRARIES ${STB_LIB_DIR})
elseif(WIN32)
  # https://cmake.org/cmake/help/git-stage/policy/CMP0110.html
  if(POLICY CMP0110)
    cmake_policy(SET CMP0110 NEW)
  endif()

  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
  set(LINK_LIBRARIES "d3d12.lib" "dxgi.lib" "d3dcompiler.lib" "dxguid.lib")
  set(INCLUDE_LIBRARIES ${STB_LIB_DIR} "${VENDOR_DIR}/d3dx12")
else()
  message(SEND_ERROR "Not supported platform!")
endif()

# LibGLaDOS
file(GLOB_RECURSE LIB_GLADOS_SOURCE_FILES
        "${LIB_GLADOS_SOURCE_DIR}/*.h"
        "${LIB_GLADOS_SOURCE_DIR}/*.hpp"
        "${LIB_GLADOS_SOURCE_DIR}/*.cpp")
if(APPLE)
  file(GLOB_RECURSE APPLE_MM_SOURCE_FILES "${LIB_GLADOS_SOURCE_DIR}/*.mm")
  foreach(SOURCE ${APPLE_MM_SOURCE_FILES})
    list(APPEND LIB_GLADOS_SOURCE_FILES ${SOURCE})
  endforeach()
endif()
add_library(${PROJECT_NAME} STATIC ${LIB_GLADOS_SOURCE_FILES})
target_link_libraries(${PROJECT_NAME}
        PRIVATE
        ${LINK_LIBRARIES}
        project_options
        project_warnings)
target_include_directories(${PROJECT_NAME} PRIVATE ${LIB_GLADOS_SOURCE_DIR} ${INCLUDE_LIBRARIES})
configure_file("${CMAKE_SOURCE_DIR}/cmake/RootDir.h.in" "${LIB_GLADOS_SOURCE_DIR}/RootDir.h")
configure_file("${CMAKE_SOURCE_DIR}/cmake/Config.h.in" "${LIB_GLADOS_SOURCE_DIR}/Config.h")

# LibGUI
file(GLOB_RECURSE LIB_GUI_SOURCE_FILES
        "${LIB_GUI_SOURCE_DIR}/*.h"
        "${LIB_GUI_SOURCE_DIR}/*.hpp"
        "${LIB_GUI_SOURCE_DIR}/*.cpp")
add_library("GUI" STATIC ${LIB_GUI_SOURCE_FILES})
target_link_libraries("GUI"
        PRIVATE
        ${LINK_LIBRARIES}
        ${PROJECT_NAME}
        project_options
        project_warnings)
target_include_directories("GUI" PRIVATE ${LIB_GLADOS_SOURCE_DIR} ${INCLUDE_LIBRARIES} ${LIB_GUI_SOURCE_DIR})

# Macro to preserve source files hierarchy in the IDE
if(MSVC_IDE)
  macro(GroupSources curdir)
    file(GLOB children RELATIVE ${PROJECT_SOURCE_DIR}/${curdir} ${PROJECT_SOURCE_DIR}/${curdir}/*)

    foreach(child ${children})
      if(IS_DIRECTORY ${PROJECT_SOURCE_DIR}/${curdir}/${child})
        GroupSources(${curdir}/${child})
      else()
        string(REPLACE "/" "\\" groupname ${curdir})
        string(REPLACE "src" "src" groupname ${groupname})
        source_group(${groupname} FILES ${PROJECT_SOURCE_DIR}/${curdir}/${child})
      endif()
    endforeach()
  endmacro()

  # Run macro
  GroupSources(src)
endif()

if (GLADOS_ENABLE_SAMPLE)
  function(make_sample TARGET ENTRYPOINT)
    source_group("" FILES "${SAMPLE_DIR}/${ENTRYPOINT}")
    add_executable(${TARGET} "${SAMPLE_DIR}/${ENTRYPOINT}")
    target_link_libraries(${TARGET} PRIVATE ${PROJECT_NAME})
    target_include_directories(${TARGET} PRIVATE ${LIB_GLADOS_SOURCE_DIR})
  endfunction()

  make_sample(00-basic "00-basic/main.cpp")
  make_sample(01-setup "01-setup/main.cpp")
  make_sample(02-camera "02-camera/main.cpp")
  make_sample(03-texture "03-texture/main.cpp")
  make_sample(04-rotateObject "04-rotateObject/main.cpp")
  make_sample(05-index "05-index/main.cpp")
  make_sample(06-skybox "06-skybox/main.cpp")
  make_sample(07-light "07-light/main.cpp")
  make_sample(08-coordinate "08-coordinate/main.cpp")
  make_sample(09-sprite "09-sprite/main.cpp")
endif ()
