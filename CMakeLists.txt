cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

include(FetchContent)

FetchContent_Declare(
  CMakeModules
  GIT_REPOSITORY "https://gitea.zimo.at:3000/ZIMO/CMakeModules.git"
  GIT_TAG v0.9.5
  SOURCE_DIR ${CMAKE_BINARY_DIR}/CMakeModules)
FetchContent_MakeAvailable(CMakeModules)

version_from_git()

project(
  libKLUG
  VERSION ${VERSION_FROM_GIT}
  LANGUAGES CXX C ASM)

set(TARGET KLUG)

add_library(${TARGET} SHARED src/new_port.cpp src/internal/transmission.cpp
                             src/internal/connection.cpp)
# add_executable( ${TARGET} test.cpp src/new_port.cpp
# src/internal/transmission.cpp src/internal/connection.cpp)

if(ANDROID)
  target_sources(${TARGET} PUBLIC src/android_wrapper.cpp)
endif()

target_compile_features(${TARGET} PUBLIC cxx_std_23)
target_include_directories(${TARGET} PRIVATE inc)

if(NOT TARGET usb-1.0)
  cpmaddpackage(
    NAME
    usb-1.0
    GITHUB_REPOSITORY
    libusb/libusb-cmake
    GIT_TAG
    v1.0.29-0
    OPTIONS
    BUILD_SHARED_LIBS
    ON)
endif()

if(NOT TARGET ULF::MDU_EIN)
  cpmaddpackage("gh:ZIMO-Elektronik/ULF_MDU_EIN@0.2.0")
endif()

if(NOT TARGET ULF::SUSIV2)
  cpmaddpackage("gh:ZIMO-Elektronik/ULF_SUSIV2@0.3.2")
endif()

find_library(log-lib log)
target_link_libraries(${TARGET} $<$<BOOL:${log-lib}>:${log-lib}> usb-1.0
                      ULF::MDU_EIN ULF::SUSIV2)
