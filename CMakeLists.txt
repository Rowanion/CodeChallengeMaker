############################################################
## Copyright(c) 2023 Timo Wiesemann. All rights reserved. ##
############################################################

cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

include(cmake/PolicySetup.cmake)

project(CodeChallengeMaker VERSION 0.0.1 LANGUAGES CXX)

##################################################################################
## add own cmake modules for convinience Functions
list( APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" "${CMAKE_SOURCE_DIR}/cmake/Modules" )

###########################################################################################
## Versioning
set( ${PROJECT_NAME}_VERSION_MAJOR 0 )
set( ${PROJECT_NAME}_VERSION_MINOR 1 )
set( ${PROJECT_NAME}_VERSION_PATCH 0 )
set( APP_VERSION_MAJOR ${${PROJECT_NAME}_VERSION_MAJOR} )
set( APP_VERSION_MINOR ${${PROJECT_NAME}_VERSION_MINOR} )
set( APP_VERSION_PATCH ${${PROJECT_NAME}_VERSION_PATCH} )

set( ${PROJECT_NAME}_VERSION ${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}.${${PROJECT_NAME}_VERSION_PATCH} )
set( APP_VERSION ${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}.${${PROJECT_NAME}_VERSION_PATCH} )

message( STATUS "Project '${PROJECT_NAME}' Version ${${PROJECT_NAME}_VERSION}" )

# change default value for install target
if( NOT CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT )
   set( CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install CACHE PATH
      "Install path prefix, prepend onto install directories."
   )
endif()

##################################################################################
## system setup
include( VariableFunctions )

include( SystemFunctions )
init_system()

include( GlobalCompilerSetup )
global_compiler_setup()

include( BuildTypes )
setup_build_types( BUILD_TYPES Debug Release DEFAULT_BUILD_TYPE Debug )

##################################################################################
## include our libs first
add_subdirectory(libs/platform)
add_subdirectory(libs/common)
add_subdirectory(libs/iplugin)

##################################################################################
## include the actual projects
add_subdirectory(CodeChallengeMaker)

##################################################################################
## include plugins
add_subdirectory(example/ExamplePlugin)

## include (all) plugins
file(GLOB_RECURSE PLUGIN_DIRS LIST_DIRECTORIES false CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/plugins*/CMakeLists.txt)
foreach(PLUGIN_DIR ${PLUGIN_DIRS})
   get_filename_component(PLUGIN_DIR_NAME ${PLUGIN_DIR} DIRECTORY )
   add_subdirectory(${PLUGIN_DIR_NAME})
endforeach()


