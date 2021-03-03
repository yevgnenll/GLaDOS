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
set(SHADER_DIR "${CMAKE_SOURCE_DIR}/shader")
set(SCRIPT_DIR "${CMAKE_SOURCE_DIR}/script")
set(SAMPLE_DIR "${CMAKE_SOURCE_DIR}/sample")
set(VENDOR_DIR "${CMAKE_SOURCE_DIR}/vendor")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(MEMORY_DEBUG 1)
else()
  set(MEMORY_DEBUG 0)
endif()

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

enable_testing()
add_subdirectory(test)

if(APPLE)
  find_library(COCOA_LIBRARY Cocoa REQUIRED)
  find_library(QUARTZCORE_LIBRARY QuartzCore REQUIRED)
  find_library(METAL_LIBRARY Metal REQUIRED)
  find_package(Threads)
  set(LINK_LIBRARIES ${COCOA_LIBRARY} ${QUARTZCORE_LIBRARY} ${METAL_LIBRARY})
elseif(UNIX AND NOT APPLE) # LINUX
  find_library(X11_LIBRARY X11 REQUIRED)
  find_library(OPENGL_LIBRARY GL REQUIRED)
  find_package(Threads)
  set(LINK_LIBRARIES ${X11_LIBRARY} ${OPENGL_LIBRARY})
elseif(WIN32)
  find_package(OpenGL REQUIRED)
  find_package(WindowsSDK REQUIRED)
  set(LINK_LIBRARIES opengl32)
else()
  message(SEND_ERROR "Not supported platform!")
endif()

file(GLOB_RECURSE SOURCE_FILES
        "${SOURCE_DIR}/*.h"
        "${SOURCE_DIR}/*.hpp"
        "${SOURCE_DIR}/*.cpp"
        "${SOURCE_DIR}/*.mm")
add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME}
        PRIVATE
        ${LINK_LIBRARIES}
        project_options
        project_warnings)
target_include_directories(${PROJECT_NAME} PRIVATE ${SOURCE_DIR})
configure_file("${CMAKE_SOURCE_DIR}/cmake/RootDir.h.in" "${SOURCE_DIR}/RootDir.h")
configure_file("${CMAKE_SOURCE_DIR}/cmake/Config.h.in" "${SOURCE_DIR}/Config.h")

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

function(make_sample TARGET ENTRYPOINT)
  source_group("" FILES "${SAMPLE_DIR}/${ENTRYPOINT}")
  add_executable(${TARGET} "${SAMPLE_DIR}/${ENTRYPOINT}")
  target_link_libraries(${TARGET} PRIVATE ${PROJECT_NAME})
  target_include_directories(${TARGET} PRIVATE ${SOURCE_DIR})
endfunction()

make_sample(01-setup "01-setup/main.cpp")
make_sample(02-camera "02-camera/main.cpp")
