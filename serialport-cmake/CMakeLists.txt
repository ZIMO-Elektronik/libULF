cmake_minimum_required(VERSION 3.15)
project(libserialport_wrapper C)

include(ExternalProject)

message(STATUS Penis)

if(CMAKE_CROSSCOMPILING)
  set(AUTOTOOLS_HOST_FLAG "--host=${CMAKE_LIBRARY_ARCHITECTURE}")
else()
  set(AUTOTOOLS_HOST_FLAG "")
endif()

set(LIBSERIALPORT_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/libserialport_ext")
set(LIBSERIALPORT_INSTALL_DIR "${LIBSERIALPORT_PREFIX}/install")

set(LIBSERIALPORT_INCLUDE_DIR "${LIBSERIALPORT_INSTALL_DIR}/include")
set(LIBSERIALPORT_LIBRARY "${LIBSERIALPORT_INSTALL_DIR}/lib/libserialport.so")

set_source_files_properties("${LIBSERIALPORT_LIBRARY}" PROPERTIES GENERATED TRUE)


ExternalProject_Add(libserialport_project
  GIT_REPOSITORY "https://github.com/sigrokproject/libserialport.git"
  GIT_TAG master
  PREFIX "${LIBSERIALPORT_PREFIX}"
  INSTALL_DIR "${LIBSERIALPORT_INSTALL_DIR}"
  CONFIGURE_COMMAND <SOURCE_DIR>/autogen.sh
  BUILD_COMMAND <SOURCE_DIR>/configure
  --prefix=<INSTALL_DIR>
  --disable-static
  --enable-shared
  ${AUTOTOOLS_HOST_FLAG}
  CC=${CMAKE_C_COMPILER}
  CFLAGS=${CMAKE_C_FLAGS}
  LDFLAGS=${CMAKE_EXE_LINKER_FLAGS}
  COMMAND make
  INSTALL_COMMAND make install
  BUILD_BYPRODUCTS "${LIBSERIALPORT_LIBRARY}"
  CMAKE_ARGS -DBUILD_SHARED_LIBS=ON
)

# Target im Unterprojekt definieren (wird für das Hauptprojekt sichtbar)
add_library(libserialport UNKNOWN IMPORTED GLOBAL)
add_dependencies(libserialport libserialport_project)

set(LIBSERIALPORT_INCLUDE_DIR "${LIBSERIALPORT_INSTALL_DIR}/include")
set(LIBSERIALPORT_LIBRARY "${LIBSERIALPORT_INSTALL_DIR}/lib/libserialport.so")

# Wichtig bei FetchContent: Ordner existieren beim ersten Aufruf noch nicht,
# daher legen wir sie virtuell an, damit CMake nicht meckert.
file(MAKE_DIRECTORY "${LIBSERIALPORT_INCLUDE_DIR}")

set_target_properties(libserialport PROPERTIES
  IMPORTED_LOCATION "${LIBSERIALPORT_LIBRARY}"
  INTERFACE_INCLUDE_DIRECTORIES "${LIBSERIALPORT_INCLUDE_DIR}"
)
