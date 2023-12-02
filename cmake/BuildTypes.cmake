############################################################
## Copyright(c) 2023 Timo Wiesemann. All rights reserved. ##
############################################################

##################################################################################
# configures the available build types (i.e Debug, Release, MinSizeRel, etc)
# usage: setup_build_types( BUILD_TYPES Release Debug DEFAULT_BUILD_TYPE Debug )
function( setup_build_types )
  set( options )
  set( oneValueArgs DEFAULT_BUILD_TYPE )
  set( multiValueArgs BUILD_TYPES )
  cmake_parse_arguments( _arg "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )

  # error and sanity checking
  if( "${_arg_DEFAULT_BUILD_TYPE}" STREQUAL "" )
    message( FATAL_ERROR "setup_build_types: missing DEFAULT_BUILD_TYPE" )
  endif()

  if( "${_arg_BUILD_TYPES}" STREQUAL "" )
    message( FATAL_ERROR "setup_build_types: missing BUILD_TYPES" )
  endif()

  if( NOT _arg_DEFAULT_BUILD_TYPE IN_LIST _arg_BUILD_TYPES )
    message( FATAL_ERROR "setup_build_types: DEFAULT_BUILD_TYPE ${_arg_DEFAULT_BUILD_TYPE} not in BUILD_TYPES ${_arg_BUILD_TYPES}" )
  endif()

  # setting the build types and
  if( NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES )
    set_property( CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS ${_arg_BUILD_TYPES} )
    set(CMAKE_BUILD_TYPE "${_arg_DEFAULT_BUILD_TYPE}" CACHE STRING "Choose the type of build" FORCE  )
    message( STATUS "Setting build type to ${arg_DEFAULT_BUILD_TYPE}" )
  endif()

  if( CMAKE_BUILD_TYPE )
    message( STATUS "Build Type: ${CMAKE_BUILD_TYPE}" )
  endif()

  if( CMAKE_CONFIGURATION_TYPES )
    set( CMAKE_CONFIGURATION_TYPES ${_arg_BUILD_TYPES} CACHE STRING "Available Build Configurations" FORCE )
  endif()  
endfunction()
