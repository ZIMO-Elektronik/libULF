cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

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
  libULF
  VERSION ${VERSION_FROM_GIT}
  LANGUAGES CXX C ASM)

set(TARGET ULF)

option(LIBULF_ENABLE_STDOUT_LOGGING "Enable logging to stdout" OFF)
option(LIBULF_ENABLE_STDERR_LOGGING "Enable logging to stderr" ON)

if(WIN32)
  # On windows, we should use Libserialport (because windows is a nice os)
  option(LIBULF_BACKEND_LIBUSB "Libusb as backend" OFF)
  option(LIBULF_BACKEND_LIBSERIALPORT "Libserialport as backend" ON)
elseif(ANDROID)
  # On android, we shoud use Libusb, since we cant open a Fd with Libserialport
  option(LIBULF_BACKEND_LIBUSB "Libusb as backend" ON)
  option(LIBULF_BACKEND_LIBSERIALPORT "Libserialport as backend" OFF)
else()
  # Default (for now)
  option(LIBULF_BACKEND_LIBUSB "Libusb as backend" OFF)
  option(LIBULF_BACKEND_LIBSERIALPORT "Libserialport as backend" ON)
endif()

if((LIBULF_BACKEND_LIBUSB AND LIBULF_BACKEND_LIBSERIALPORT)
   OR (NOT LIBULF_BACKEND_LIBUSB AND NOT LIBULF_BACKEND_LIBSERIALPORT))
  message(
    FATAL_ERROR
      "Exactly ONE backend must be selected (Either LIBUSB or LIBSERIALPORT)")
endif()

if(LIBULF_BACKEND_LIBUSB AND WIN32)
  message(
    WARNING
      "Libusb will work on windows ONLY if the WinUSB driver is used (not by default)"
  )
endif()

if(LIBULF_BACKEND_LIBSERIALPORT AND ANDROID)
  message(
    WARNING
      "Libserialport may not work on Android, since there is no way to pass a file descriptor"
  )
endif()

add_library(
  ${TARGET} SHARED
  src/transmission/transmission_base.cpp
  src/transmission/com/base.cpp
  src/transmission/mdu_ein/base.cpp
  src/transmission/mdu_ein/cv_read.cpp
  src/transmission/mdu_ein/config_transfer_rate.cpp
  src/transmission/mdu_ein/ping.cpp
  src/transmission/susiv2/base.cpp
  src/transmission/susiv2/cv_read.cpp
  src/bridge/bridge.cpp
  src/bridge/bridge_com.cpp
  src/bridge/bridge_context.cpp
  src/bridge/bridge_susiv2.cpp
  src/bridge/bridge_mdu_ein.cpp
  src/bridge/bridge_zpp.cpp
  src/bridge/bridge_zsu.cpp
  src/libulf.cpp)

if(LIBULF_BACKEND_LIBUSB)
  target_sources(${TARGET} PRIVATE src/connection/libusb_connection.cpp)
  target_compile_definitions(${TARGET} PRIVATE USE_LIBUSB)
elseif(LIBULF_BACKEND_LIBSERIALPORT)
  target_sources(${TARGET} PRIVATE src/connection/libserialport_connection.cpp)
  target_compile_definitions(${TARGET} PRIVATE USE_LIBSERIALPORT)
endif()

target_compile_features(${TARGET} PUBLIC cxx_std_23)
target_include_directories(${TARGET} PUBLIC include/)
target_include_directories(${TARGET} PRIVATE src)

target_compile_definitions(${TARGET} PRIVATE JNI_CLASS_PATH=${JNI_CLASS_PATH}
                                             JNI_RESULT_PATH=${JNI_RESULT_PATH})

if(NOT TARGET usb-1.0 AND LIBULF_BACKEND_LIBUSB)
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
elseif(NOT TARGET libserialport AND LIBULF_BACKEND_LIBSERIALPORT)
  add_subdirectory(serialport-cmake)
endif()

if(NOT TARGET MDU::MDU)
  cpmaddpackage("gh:ZIMO-Elektronik/MDU@0.19.3")
endif()

if(NOT TARGET ULF::MDU_EIN)
  cpmaddpackage("gh:ZIMO-Elektronik/ULF_MDU_EIN@0.2.0")
endif()

if(NOT TARGET ULF::SUSIV2)
  cpmaddpackage("gh:ZIMO-Elektronik/ULF_SUSIV2@0.3.5")
endif()

if(NOT TARGET ZPP::ZPP)
  cpmaddpackage("gh:ZIMO-Elektronik/ZPP@0.2.1")
endif()

if(NOT TARGET ZSU::ZSU)
  cpmaddpackage("gh:ZIMO-Elektronik/ZSU@0.1.1")
endif()

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

to_cpp_bool(${LIBULF_ENABLE_STDOUT_LOGGING} LOG_TO_STDOUT_VALUE)
to_cpp_bool(${LIBULF_ENABLE_STDERR_LOGGING} LOG_TO_STDERR_VALUE)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/template/config.hpp.in
               ${CMAKE_CURRENT_BINARY_DIR}/generated/config.hpp @ONLY)

target_include_directories(${TARGET}
                           PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/generated)

find_library(log-lib log)
target_link_libraries(
  ${TARGET}
  $<$<BOOL:${log-lib}>:${log-lib}>
  ULF::MDU_EIN
  ULF::SUSIV2
  ZPP::ZPP
  ZSU::ZSU
  "$<$<BOOL:${LIBULF_BACKEND_LIBSERIALPORT}>:libserialport>"
  "$<$<BOOL:${LIBULF_BACKEND_LIBUSB}>:usb-1.0>")

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
  add_clang_format_target(ULFFormat OPTIONS -i FILES ${SRC})
  add_include_what_you_must_target(
    ULFIncludeWhatYouMust TARGET ULF EXCLUDE_HEADERS
    src/connection/libusb_connection.hpp
    src/connection/libserialport_connection.hpp)
  target_include_directories(
    ULFIncludeWhatYouMust PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/generated/ src/)
endif()

set(INSTALL_TARGETS ULF)

if(LIBULF_BACKEND_LIBSERIALPORT)
  list(APPEND INSTALL_TARGETS libserialport)
endif()

if(LIBULF_BACKEND_LIBUSB)
  list(APPEND INSTALL_TARGETS usb-1.0)
endif()

install(
  TARGETS ${INSTALL_TARGETS}
  LIBRARY DESTINATION lib # .so-Files
  RUNTIME DESTINATION bin # .dll-Files (to avoid linking issues)
  ARCHIVE DESTINATION lib # [.a/.lib]-Files
)
