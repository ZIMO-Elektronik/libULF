cmake_minimum_required(VERSION 3.15)
project(serialport C)

include(FetchContent)
include(CheckIncludeFiles)

# Fetch source manually
FetchContent_Declare(
  libserialport
  GIT_REPOSITORY "https://github.com/sigrokproject/libserialport.git"
  GIT_TAG "master")
FetchContent_MakeAvailable(libserialport)

check_include_files(sys/time.h HAVE_SYS_TIME_H)
check_include_files(sys/types.h HAVE_SYS_TYPES_H)
check_include_files(unistd.h HAVE_UNISTD_H)
check_include_files(termios.h HAVE_TERMIOS_H)

# Generate config
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/config.h.in"
               "${CMAKE_CURRENT_BINARY_DIR}/config.h")

add_library(serialport SHARED "${libserialport_SOURCE_DIR}/serialport.c"
                              "${libserialport_SOURCE_DIR}/timing.c")

if(WIN32)
  # Include platform header
  target_sources(serialport PRIVATE "${libserialport_SOURCE_DIR}/windows.c")
elseif(UNIX)
  if(APPLE)
    # Include platform header
    target_sources(serialport PRIVATE "${libserialport_SOURCE_DIR}/macosx.c")

    # Link platform frameworks
    find_library(COREFOUNDATION_FRAMEWORK CoreFoundation REQUIRED)
    find_library(IOKIT_FRAMEWORK IOKit REQUIRED)
    target_link_libraries(serialport PRIVATE ${COREFOUNDATION_FRAMEWORK}
                                             ${IOKIT_FRAMEWORK})
  elseif(LINUX)
    # Include platform header
    target_sources(serialport PRIVATE "${libserialport_SOURCE_DIR}/linux.c")
  endif()
endif()

# Define macros
target_compile_definitions(
  serialport
  PRIVATE _GNU_SOURCE LIBSERIALPORT_BUILDING "SP_API=__declspec\(dllexport\)"
          "SP_PRIV=" "SP_PACKAGE_VERSION_STRING=\"0.1.2\""
          "SP_LIB_VERSION_STRING=\"1:1:1\"")

# Define search path
target_include_directories(
  serialport
  PUBLIC "${libserialport_SOURCE_DIR}"
  PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

# Link windows usb libs
if(WIN32)
  target_link_libraries(serialport PRIVATE setupapi uuid)

  # Force export of DLL
  set_target_properties(
    serialport
    PROPERTIES PREFIX ""
               IMPORT_PREFIX ""
               OUTPUT_NAME "libserialport")
endif()
