cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
include(FetchContent)

FetchContent_Declare(
  CMakeModules
  GIT_REPOSITORY "https://gitea.zimo.at:3000/ZIMO/CMakeModules.git"
  GIT_TAG v0.12.2
  SOURCE_DIR ${CMAKE_BINARY_DIR}/CMakeModules)
FetchContent_MakeAvailable(CMakeModules)

version_from_git()

project(
  libKLUG
  VERSION ${VERSION_FROM_GIT}
  LANGUAGES CXX C ASM)

set(TARGET KLUG)

set(SOURCE_FILES
    src/libklug/internal/connection.cpp
    src/libklug/internal/functor.cpp
    src/libklug/internal/transmission/transmission_base.cpp
    src/libklug/internal/transmission/com/base.cpp
    src/libklug/internal/transmission/mdu_ein/base.cpp
    src/libklug/internal/transmission/mdu_ein/cv_read.cpp
    src/libklug/internal/transmission/mdu_ein/ping.cpp
    src/libklug/internal/transmission/susiv2/base.cpp
    src/libklug/internal/transmission/susiv2/cv_read.cpp
    src/libklug/bridge/internal/bridge.cpp
    src/libklug/bridge/internal/bridge_com.cpp
    src/libklug/bridge/internal/bridge_context.cpp
    src/libklug/bridge/internal/bridge_susiv2.cpp
    src/libklug/bridge/internal/bridge_mdu_ein.cpp
    src/libklug/bridge/internal/bridge_worker.cpp
    src/libklug/bridge/internal/bridge_zpp.cpp
    src/libklug/libklug.cpp)

add_library(${TARGET} SHARED ${SOURCE_FILES})
# add_executable(${TARGET} test.cpp ${SOURCE_FILES})

if(ANDROID)
  target_sources(
    ${TARGET}
    PUBLIC src/libklug/libklug_jni.cpp src/libklug/internal/android_funktor.cpp
           src/libklug/android_result.cpp src/libklug/internal/jni_context.cpp)
endif()

target_compile_features(${TARGET} PUBLIC cxx_std_23)
target_include_directories(${TARGET} PUBLIC inc/)

target_compile_definitions(${TARGET} PRIVATE JNI_CLASS_PATH=${JNI_CLASS_PATH}
                                             JNI_RESULT_PATH=${JNI_RESULT_PATH})

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

if(NOT TARGET ZPP::ZPP)
  cpmaddpackage("gh:ZIMO-Elektronik/ZPP@0.2.1")
endif()

if(NOT TARGET ZSU::ZSU)
  cpmaddpackage("gh:ZIMO-Elektronik/ZSU@0.1.1")
endif()

find_library(log-lib log)
target_link_libraries(
  ${TARGET}
  $<$<BOOL:${log-lib}>:${log-lib}>
  usb-1.0
  ULF::MDU_EIN
  ULF::SUSIV2
  ZPP::ZPP
  ZSU::ZSU)

if(PROJECT_IS_TOP_LEVEL)
  add_subdirectory(examples)
  file(
    DOWNLOAD
    "https://github.com/ZIMO-Elektronik/.github/raw/master/data/.clang-format"
    ${CMAKE_CURRENT_LIST_DIR}/.clang-format)
  file(GLOB_RECURSE SRC include/*.*pp src/*.*pp tests/*.*pp)
  add_clang_format_target(KLUGFormat OPTIONS -i FILES ${SRC})
endif()
