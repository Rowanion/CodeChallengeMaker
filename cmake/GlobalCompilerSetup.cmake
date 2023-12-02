############################################################
## Copyright(c) 2023 Timo Wiesemann. All rights reserved. ##
############################################################

##################################################################################
## configure compiler options for specific compilers
function( global_compiler_setup )

  set( CMAKE_CXX_STANDARD 17 )
  set( CMAKE_CXX_STANDARD_REQUIRED ON )
  set( CMAKE_CXX_EXTENSIONS OFF )

  if( WIN32 )
    #Get rid of annoying windows defines, 
    # old win95 relic to reduce includes from windows.h
    list( APPEND GLOBAL_COMPILE_DEFINITIONS WIN32_LEAN_AND_MEAN )
		#NOMINMAX to avoid the min max problems
    list(APPEND GLOBAL_COMPILE_DEFINITIONS NOMINMAX)
		#NOGID    the gdi header define a GetObject macro that screws with a lot of things, e.g. rapidjson 
    list(APPEND GLOBAL_COMPILE_DEFINITIONS NOGDI)

    # This is inside WindowsPlatform.cpp +getExecutablePath() and needs to be addressed soon.
    list(APPEND GLOBAL_COMPILE_DEFINITIONS _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING)
  endif()

  if( IS_PROJECT_COMPILER_MSVC )
    # enable multi-process-compiling
    list( APPEND GLOBAL_COMPILE_OPTIONS /MP )

    # add extra warnings and security features
    list( APPEND GLOBAL_COMPILE_OPTIONS /sdl )

    # be 'relatively' pedantic
    list( APPEND GLOBAL_COMPILE_OPTIONS /W4 )

    # disable warnings for unreferenced parameters
    list( APPEND GLOBAL_COMPILE_OPTIONS /wd4100 )

  endif()

  if( IS_PROJECT_COMPILER_CLANG )

    # add extra warnings
    list( APPEND GLOBAL_COMPILE_OPTIONS -Wextra )

    # enable a lot of warnings
    list( APPEND GLOBAL_COMPILE_OPTIONS -Wall )

    # enable warning when using non-virtual DTOR in class w/ virtual functions.
    list(APPEND GLOBAL_COMPILE_OPTIONS -Wnon-virtual-dtor)

    ## ########################################
    ## disabling certain warnings

    # disable warnings for unreferenced parameters
    list( APPEND GLOBAL_COMPILE_OPTIONS -Wno-unused-parameter )

    # disable warnings for scalar initializers like int i = {7};
    list( APPEND GLOBAL_COMPILE_OPTIONS -Wno-braced-scalar-init )

    # disable warnings for c++98 compatibility breaks
    list( APPEND GLOBAL_COMPILE_OPTIONS -Wno-c++98-compat -Wno-c++98-compat-extra-semi -Wno-c++98-compat-pedantic )

    # disable warnings for (missing) static CTORs and DTORs
    list( APPEND GLOBAL_COMPILE_OPTIONS -Wno-exit-time-destructors -Wno-global-constructors )

    # disable warnings for unnecessary semicolons
    list( APPEND GLOBAL_COMPILE_OPTIONS -Wno-extra-semi )

    # disable warnings for init member re-ordering
    list( APPEND GLOBAL_COMPILE_OPTIONS -Wno-reorder-ctor )

    # disable warnings for missing enum values in switch-case
    list( APPEND GLOBAL_COMPILE_OPTIONS -Wno-switch )

    # these warnings are a) a bit weird and b) they come mostly from 3rdParty headers
    list( APPEND GLOBAL_COMPILE_OPTIONS 
      -Wno-language-extension-token
      -Wno-reserved-id-macro 
      -Wno-gnu-zero-variadic-macro-arguments
    )

    # show debug info in lldb
    list( APPEND GLOBAL_COMPILE_OPTIONS_DEBUG -fstandalone-debug )
  endif()

  # hand these variables over to calling script's scope
  set( GLOBAL_COMPILE_OPTIONS ${GLOBAL_COMPILE_OPTIONS} PARENT_SCOPE )
  set( GLOBAL_COMPILE_OPTIONS_DEBUG ${GLOBAL_COMPILE_OPTIONS_DEBUG} PARENT_SCOPE )
  set( GLOBAL_COMPILE_DEFINITIONS ${GLOBAL_COMPILE_DEFINITIONS} PARENT_SCOPE )
  set( CMAKE_CXX_STANDARD ${CMAKE_CXX_STANDARD} PARENT_SCOPE )
  set( CMAKE_CXX_STANDARD_REQUIRED ${CMAKE_CXX_STANDARD_REQUIRED} PARENT_SCOPE )
  set( CMAKE_CXX_EXTENSIONS ${CMAKE_CXX_EXTENSIONS} PARENT_SCOPE )


endfunction()