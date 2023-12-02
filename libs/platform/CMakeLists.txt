############################################################
## Copyright(c) 2023 Timo Wiesemann. All rights reserved. ##
############################################################

project(platform)

######################################################
## This is a modular compiler/system setup
#   It is able to handle different compiler-setups.
#   For now everything is exported.

include(${CMAKE_SOURCE_DIR}/cmake/CompilerSetup.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/OsSetup.cmake)

########################################################
## System setup
if(COMPILER_GCC)
    list(APPEND _public_definitions COMPILER_GCC=1)
    list(APPEND ${PROJECT_NAME}_SOURCES ${CMAKE_CURRENT_LIST_DIR}/compiler/GCCCompiler.h ${CMAKE_CURRENT_LIST_DIR}/compiler/GCCCompiler.cpp)
elseif(COMPILER_CLANG)
    list(APPEND _public_definitions COMPILER_CLANG=1)
    list(APPEND ${PROJECT_NAME}_SOURCES ${CMAKE_CURRENT_LIST_DIR}/compiler/ClangCompiler.h ${CMAKE_CURRENT_LIST_DIR}/compiler/ClangCompiler.cpp)
elseif(COMPILER_MSVC)
    list(APPEND _public_definitions COMPILER_MSVC=1)
    list(APPEND ${PROJECT_NAME}_SOURCES ${CMAKE_CURRENT_LIST_DIR}/compiler/MSVCCompiler.h ${CMAKE_CURRENT_LIST_DIR}/compiler/MSVCCompiler.cpp)
endif()

if(OS_LINUX)
    list(APPEND _public_definitions OS_LINUX=1)
    list(APPEND ${PROJECT_NAME}_SOURCES ${CMAKE_CURRENT_LIST_DIR}/os/LinuxPlatform.h ${CMAKE_CURRENT_LIST_DIR}/os/LinuxPlatform.cpp)
elseif(OS_WINDOWS)
    list(APPEND _public_definitions OS_WINDOWS=1)
    list(APPEND ${PROJECT_NAME}_SOURCES ${CMAKE_CURRENT_LIST_DIR}/os/WindowsPlatform.h ${CMAKE_CURRENT_LIST_DIR}/os/WindowsPlatform.cpp)
endif()

# general sources
list(APPEND 
    ${PROJECT_NAME}_SOURCES 
    ${CMAKE_CURRENT_LIST_DIR}/ErrnoGuard.h
    ${CMAKE_CURRENT_LIST_DIR}/ErrnoGuard.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Globals.h
    ${CMAKE_CURRENT_LIST_DIR}/SystemTraits.h
    ${CMAKE_CURRENT_LIST_DIR}/SystemTraits.cpp
)

# add the sources to the source group to avoid header/source separation
source_group( TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX src FILES ${${PROJECT_NAME}_SOURCES} )

#########################################################
## The library
add_library(${PROJECT_NAME} OBJECT
    ${${PROJECT_NAME}_SOURCES}
)

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Libs")

# this is required so we can link this against .so/.dll AND static
set_property(TARGET ${PROJECT_NAME} PROPERTY POSITION_INDEPENDENT_CODE ON)

target_compile_options( ${PROJECT_NAME}
    PUBLIC
        ${GLOBAL_COMPILE_OPTIONS}
    PUBLIC
        $<$<CONFIG:Debug>:${GLOBAL_COMPILE_OPTIONS_DEBUG}>
)

target_compile_definitions(${PROJECT_NAME}
    PUBLIC
        ${GLOBAL_COMPILE_DEFINITIONS}
        ${_public_definitions}
        OS_NAME="${CMAKE_SYSTEM_NAME}"
    PRIVATE 
        COMPILER_NAME="${COMPILER_NAME}"
        COMPILER_VERSION="${COMPILER_VERSION}"
)

target_include_directories(${PROJECT_NAME} 
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
)
