# Set variables to Windows on AMD64
set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR AMD64)

# Set clang as compiler
set(CMAKE_C_COMPILER clang)
set(CMAKE_C_COMPILER_TARGET x86_64-w64-windows-gnu)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_COMPILER_TARGET x86_64-w64-windows-gnu)

# Comply with mingw
set(CMAKE_C_FLAGS_INIT "-femulated-tls")
set(CMAKE_CXX_FLAGS_INIT "-femulated-tls")

# Force LLVM LLD linker
set(CMAKE_LINKER lld)
set(CMAKE_EXE_LINKER_FLAGS_INIT "-fuse-ld=lld")
set(CMAKE_SHARED_LINKER_FLAGS_INIT "-fuse-ld=lld")
