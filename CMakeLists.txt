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

set(SOURCE_FILES
    src/new_port.cpp
    src/internal/transmission.cpp
    src/internal/connection.cpp
    src/susiv2.cpp
    src/com.cpp
    src/internal/com_transmission.cpp
    src/internal/mdu_ein_transmission.cpp
    src/internal/susiv2_transmission.cpp
    src/bridge/bridge.cpp
    src/bridge/internal/bridge.cpp
    src/bridge/internal/bridge_com.cpp
    src/bridge/internal/bridge_context.cpp
    src/bridge/internal/bridge_susiv2.cpp
    src/bridge/internal/bridge_worker.cpp)

add_library(${TARGET} SHARED ${SOURCE_FILES})
# add_executable(e_${TARGET} test.cpp src/new_port.cpp ${SOURCE_FILES})

if(ANDROID)
  target_sources(${TARGET} PUBLIC src/android_wrapper.cpp
                                  src/bridge/android_bridge.cpp)
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
  cpmaddpackage("gh:ZIMO-Elektronik/ULF_SUSIV2#packet2frame")
endif()

find_library(log-lib log)
target_link_libraries(${TARGET} $<$<BOOL:${log-lib}>:${log-lib}> usb-1.0
                      ULF::MDU_EIN ULF::SUSIV2)
