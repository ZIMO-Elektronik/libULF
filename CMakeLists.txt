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
    src/libklug/internal/connection/libusb_connection.cpp
    src/libklug/internal/connection/libserialport_connection.cpp
    src/libklug/callback/functor.cpp
    src/libklug/internal/transmission/transmission_base.cpp
    src/libklug/internal/transmission/com/base.cpp
    src/libklug/internal/transmission/mdu_ein/base.cpp
    src/libklug/internal/transmission/mdu_ein/cv_read.cpp
    src/libklug/internal/transmission/mdu_ein/config_transfer_rate.cpp
    src/libklug/internal/transmission/mdu_ein/ping.cpp
    src/libklug/internal/transmission/susiv2/base.cpp
    src/libklug/internal/transmission/susiv2/cv_read.cpp
    src/libklug/internal/bridge/bridge.cpp
    src/libklug/internal/bridge/bridge_com.cpp
    src/libklug/internal/bridge/bridge_context.cpp
    src/libklug/internal/bridge/bridge_susiv2.cpp
    src/libklug/internal/bridge/bridge_mdu_ein.cpp
    src/libklug/internal/bridge/bridge_worker.cpp
    src/libklug/internal/bridge/bridge_zpp.cpp
    src/libklug/internal/bridge/bridge_zsu.cpp
    src/libklug/libklug.cpp)

add_library(${TARGET} SHARED ${SOURCE_FILES})

if(ANDROID)
  target_sources(
    ${TARGET}
    PUBLIC src/libklug/libklug_jni.cpp
           src/libklug/internal/platform/android/jni_functor.cpp
           src/libklug/internal/platform/android/jni_dispatch.cpp
           src/libklug/internal/platform/android/jni_context.cpp)
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

if(NOT TARGET MDU::MDU)
  cpmaddpackage("gh:ZIMO-Elektronik/MDU@0.19.3")
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

add_subdirectory(serialport-cmake)

option(LIBKLUG_ENABLE_STDOUT_LOGGING "Enable logging to stdout" ON)
option(LIBKLUG_ENABLE_STDERR_LOGGING "Enable logging to stderr" ON)

function(to_cpp_bool cmake_value output_var)
  if(${cmake_value})
    set(${output_var}
        "true"
        PARENT_SCOPE)
  else()
    set(${output_var}
        "false"
        PARENT_SCOPE)
  endif()
endfunction()

to_cpp_bool(${LIBKLUG_ENABLE_STDOUT_LOGGING} LOG_TO_STDOUT_VALUE)
to_cpp_bool(${LIBKLUG_ENABLE_STDERR_LOGGING} LOG_TO_STDERR_VALUE)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.hpp.in
               ${CMAKE_CURRENT_BINARY_DIR}/generated/config.hpp @ONLY)

target_include_directories(KLUG PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/generated)

find_library(log-lib log)
target_link_libraries(
  ${TARGET}
  $<$<BOOL:${log-lib}>:${log-lib}>
  usb-1.0
  ULF::MDU_EIN
  ULF::SUSIV2
  ZPP::ZPP
  ZSU::ZSU
  libserialport)

if(PROJECT_IS_TOP_LEVEL)
  include(CTest)
  enable_testing()
  add_subdirectory(examples)
  add_subdirectory(tests)
  file(
    DOWNLOAD
    "https://github.com/ZIMO-Elektronik/.github/raw/master/data/.clang-format"
    ${CMAKE_CURRENT_LIST_DIR}/.clang-format)
  file(GLOB_RECURSE SRC include/*.*pp src/*.*pp tests/*.*pp)
  add_clang_format_target(KLUGFormat OPTIONS -i FILES ${SRC})
  add_include_what_you_must_target(
    KLUGIncludeWhatYouMust
    TARGET
    KLUG
    EXCLUDE_HEADERS
    inc//libklug/libklug_jni.hpp
    inc//libklug/internal/platform/android/jni_context.hpp
    inc//libklug/internal/platform/android/jni_defines.hpp
    inc//libklug/internal/platform/android/jni_dispatch.hpp
    inc//libklug/internal/platform/android/jni_functor.hpp)
  target_include_directories(KLUGIncludeWhatYouMust
                             PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/generated/)
endif()
