############################################################
## Copyright(c) 2023 Timo Wiesemann. All rights reserved. ##
############################################################

include_guard(GLOBAL)

# this check is order-dependent because MINGW is also GNU.
if(MINGW) 
    set(COMPILER_NAME "MINGW")
    set(COMPILER_MINGW true)
    set(CMAKE_POSITION_INDEPENDENT_CODE OFF) # this is to avoid MinGW warning; MinGW generates position-independent-code for #DLL by default
    set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} ${_mingw_link_flags})
    set(CMAKE_MODULE_LINKER_FLAGS ${CMAKE_MODULE_LINKER_FLAGS} ${_mingw_link_flags})
    set(CMAKE_SHARED_LINKER_FLAGS ${CMAKE_SHARED_LINKER_FLAGS} ${_mingw_link_flags})
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(COMPILER_NAME "GCC")
    set(COMPILER_GCC true)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(COMPILER_NAME "CLANG")
    set(COMPILER_CLANG true)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(COMPILER_NAME "MSVC")
    set(COMPILER_MSVC true)
else()
    message(FATAL_ERROR "Unsupported Compiler: \"${CMAKE_CXX_COMPILER_ID}\"")
endif()

set(COMPILER_VERSION ${CMAKE_C_COMPILER_VERSION})