############################################################
## Copyright(c) 2023 Timo Wiesemann. All rights reserved. ##
############################################################

##################################################################################
## Available variables after call:
##   BUILD_ARCHITECTURE (x32|x64)
##   IS_PROJECT_COMPILER_CLANG (true|false)
##   IS_PROJECT_COMPILER_MSVC (true|false)
##   IS_PROJECT_COMPILER_GCC (true|false)
##   IS_CROSSCOMPILING (true|false)
function( init_system )

  if( WIN32 )
    # enable virtual folders for Visual Studio
    set_property(GLOBAL PROPERTY USE_FOLDERS ON  )
  endif()

  # detect system architecture - not correct for crosscompiling
  #   can't test compile either because I wouldn't be able to run that
  if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    set( BUILD_ARCHITECTURE x64 )
  else()
    set( BUILD_ARCHITECTURE x32 )
  endif()
  message( STATUS "Build Architecture is set to ${BUILD_ARCHITECTURE}" )
  set( BUILD_ARCHITECTURE ${BUILD_ARCHITECTURE} PARENT_SCOPE )

  # detect compiler
  if( CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "AppleClang" )
    set( _is_project_compiler_clang true )
  elseif( CMAKE_CXX_COMPILER_ID MATCHES "MSVC" )
    set( _is_project_compiler_msvc true )
  elseif( CMAKE_CXX_COMPILER_ID MATCHES "GNU" )
    set( _is_project_compiler_gcc true )
  endif()

  set( IS_PROJECT_COMPILER_CLANG ${_is_project_compiler_clang} PARENT_SCOPE )
  set( IS_PROJECT_COMPILER_MSVC ${_is_project_compiler_msvc} PARENT_SCOPE )
  set( IS_PROJECT_COMPILER_GCC ${_is_project_compiler_gcc} PARENT_SCOPE )
  
  # detect crosscompiling - because setting CMAKE_CROSSCOMPILE in toolchain file is not enough
  if( NOT DEFINED CMAKE_TOOLCHAIN_FILE OR CMAKE_TOOLCHAIN_FILE STREQUAL "")
    set( IS_CROSSCOMPILING false PARENT_SCOPE )
  else()
    set( IS_CROSSCOMPILING true PARENT_SCOPE )
  endif()
endfunction()

function( get_WIN32_WINNT )
  if( CMAKE_SYSTEM_VERSION )
    set( ver ${CMAKE_SYSTEM_VERSION} )
    string( REGEX MATCH "^([0-9]+).([0-9])" ver ${ver} )
    string( REGEX MATCH "^([0-9]+)" verMajor ${ver} )
    # Check for Windows 10, b/c we'll need to convert to hex 'A'.
    if( "${verMajor}" MATCHES "10" )
      set( verMajor "A" )
      string( REGEX REPLACE "^([0-9]+)" ${verMajor} ver ${ver} )
    endif()
    # Remove all remaining '.' characters.
    string( REPLACE "." "" ver ${ver} )
    # Prepend each digit with a zero.
    string( REGEX REPLACE "([0-9A-Z])" "0\\1" ver ${ver} )
    set( WINNT_VERSION "0x${ver}" PARENT_SCOPE )
  endif()
endfunction()
