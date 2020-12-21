cmake_minimum_required(VERSION 3.16)
message(STATUS "Current CMake Version ${CMAKE_VERSION}")
project(gameengine LANGUAGES CXX VERSION 0.0.1)
message(STATUS "Build ${PROJECT_NAME} Version ${CMAKE_PROJECT_VERSION}")

set(CMAKE_CXX_STANDARD 17)
set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")
set(RESOURCE_DIR "resource")
set(SHADER_DIR "shader")
set(SCRIPT_DIR "script")
set(VENDOR_DIR "${CMAKE_SOURCE_DIR}/vendor")

# Link this 'library' to set the c++ standard / compile-time options requested
add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_17)

# Link this 'library' to use the warnings specified in CompilerWarnings.cmake
add_library(project_warnings INTERFACE)

# standard compiler warnings
include(cmake/CompilerWarnings.cmake)
set_project_warnings(project_warnings)

# Static Analyzer settings (cppcheck, clang-tidy, iwyu)
include(cmake/StaticAnalyzers.cmake)

find_program(CCACHE ccache)
if(CCACHE)
  set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE})
else()
  message(STATUS "ccache not found cannot use")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

enable_testing()
add_subdirectory(test)

if(APPLE)
  find_library(QUARTZCORE_LIBRARY QuartzCore REQUIRED)
  find_library(COCOA_LIBRARY Cocoa REQUIRED)
  find_library(METAL_LIBRARY Metal REQUIRED)
  find_package(Threads)
  set(LINK_LIBRARIES ${COCOA_LIBRARY} ${QUARTZCORE_LIBRARY} ${METAL_LIBRARY} ${CONAN_LIBS})
elseif(UNIX AND NOT APPLE) # LINUX
#  message(SEND_ERROR "TODO: not yet supported")
  set(LINK_LIBRARIES)
elseif(WIN32)
#  message(SEND_ERROR "TODO: not yet supported")
  set(LINK_LIBRARIES)
else()
  message(SEND_ERROR "Failed to detect platform")
endif()

add_executable(${PROJECT_NAME})
file(GLOB_RECURSE SOURCE_FILES
        "${SOURCE_DIR}/*.hpp"
        "${SOURCE_DIR}/*.cpp"
        "${SOURCE_DIR}/*.mm")
target_sources(${PROJECT_NAME} PRIVATE ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME}
        PRIVATE
        ${LINK_LIBRARIES}
        project_options
        project_warnings)
target_include_directories(${PROJECT_NAME} PRIVATE ${SOURCE_DIR})
configure_file("${CMAKE_SOURCE_DIR}/cmake/RootDir.h.in" "${SOURCE_DIR}/RootDir.h")

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