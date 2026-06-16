cmake_minimum_required(VERSION 3.15)
project(libserialport_wrapper C)

include(FetchContent)
include(CheckIncludeFiles)

# 1. Wir laden nur die Quellcodes herunter, anstatt ExternalProject zu nutzen
FetchContent_Declare(
  libserialport_src
  GIT_REPOSITORY "https://github.com/sigrokproject/libserialport.git"
  GIT_TAG "master")
FetchContent_MakeAvailable(libserialport_src)

check_include_files(sys/time.h HAVE_SYS_TIME_H)
check_include_files(sys/types.h HAVE_SYS_TYPES_H)
check_include_files(unistd.h HAVE_UNISTD_H)
check_include_files(termios.h HAVE_TERMIOS_H)

# 1. Die config.h aus der Vorlage generieren (sauber und unbloated)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/config.h.in"
               "${CMAKE_CURRENT_BINARY_DIR}/config.h")

# 1. Wir definieren das SHARED-Target (die DLL) direkt in CMake! libserialport
#   besteht auf Windows primär aus diesen Quelldateien.
add_library(libserialport SHARED "${libserialport_src_SOURCE_DIR}/serialport.c"
                                 "${libserialport_src_SOURCE_DIR}/timing.c")

if(WIN32)
  target_sources(libserialport
                 PRIVATE "${libserialport_src_SOURCE_DIR}/windows.c")
else()
  target_sources(libserialport
                 PRIVATE "${libserialport_src_SOURCE_DIR}/linux.c")
endif()

# 1. Interne Makros setzen, die libserialport zum Kompilieren braucht
target_compile_definitions(
  libserialport
  PRIVATE _GNU_SOURCE LIBSERIALPORT_BUILDING "SP_API=__declspec\(dllexport\)"
          "SP_PRIV=" "SP_PACKAGE_VERSION_STRING=\"0.1.2\""
          "SP_LIB_VERSION_STRING=\"1:1:1\"")

# 1. Header-Suchpfad für Ihr Hauptprojekt bereitstellen
target_include_directories(
  libserialport
  PUBLIC "${libserialport_src_SOURCE_DIR}"
  PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

# 1. Windows-Systembibliotheken linken, die für serielle Ports nötig sind
if(WIN32)
  target_link_libraries(libserialport PRIVATE setupapi uuid)

  # Zwingt CMake, die DLL sauber als DLL zu exportieren
  set_target_properties(
    libserialport
    PROPERTIES PREFIX ""
               IMPORT_PREFIX ""
               OUTPUT_NAME "libserialport")
endif()
