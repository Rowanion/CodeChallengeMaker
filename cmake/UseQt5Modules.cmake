############################################################
## Copyright(c) 2023 Timo Wiesemann. All rights reserved. ##
############################################################

# used for copyDllsOnBuild & install- list of known module names - please add more as you go
set( _qt_known_extra_plugins 
  Qt5::QXcbIntegrationPlugin
  Qt5::QWindowsIntegrationPlugin 
  Qt5::QGifPlugin 
  Qt5::QXcbGlxIntegrationPlugin
)

# used for copyDllsOnBuild & install - list of folders names where known plugins need to
#  be copied to. As CMake lacks dictionaries, the indices of these two lists HAVE to match.
set( _qt_extra_plugin_folders 
  platforms
  platforms
  imageformats 
  xcbglintegrations
)

macro( useQt5Modules add_to_link )

  find_package( Qt5 REQUIRED COMPONENTS ${ARGN} )
  set( CMAKE_AUTOMOC ON )
  set( CMAKE_AUTOUIC ON )
  set( CMAKE_AUTORCC ON )

  message( "adding Qt5 Modules: ${ARGN}" )
  message( STATUS "Qt5: ${Qt5_DIR}" )
  
  set( QT5_VERSION ${Qt5${ARGV2}_VERSION} )
  string( REPLACE "." ";"  _found_qt_version_split ${QT5_VERSION})
  list( GET _found_qt_version_split 0 QT5_VERSION_MAJOR )
  list( GET _found_qt_version_split 1 QT5_VERSION_MINOR )
  list( GET _found_qt_version_split 2 QT5_VERSION_PATCH )

  message( STATUS "Qt5 VERSION: ${QT5_VERSION}" )

  if( ${add_to_link} )
    foreach( moduleName ${ARGN} )
      list( APPEND QT5_LINK_LIBS Qt5::${moduleName} )       
    endforeach()
  endif()

endmacro()

##################################################################################
## Creates a post-build target for TARGET to copy over all used Qt dll.
##
## usage: copyQtPostBuild( TARGET MyQtBuildTarget ADDITIONAL_PLUGINS Qt5::QGifPlugin )
## Note: Takes care of linked dlls and additional plugins.
##
function( copyQtPostBuild )
  set( options )
  set( oneValueArgs TARGET )
  set( multiValueArgs ADDITIONAL_PLUGINS )
  cmake_parse_arguments( _arg "${options}" "${oneValueArgs}" "${multiValueArgs}" "${ARGN}" )

  if( "${_arg_TARGET}" STREQUAL "" )
    message( FATAL_ERROR "copyQtPostBuild: no TARGET set" )
  endif()

  if( NOT TARGET ${_arg_TARGET} )
    message( FATAL_ERROR "copyQtPostBuild: specified TARGET ${_arg_TARGET} is not a valid target." )
  endif()

  # convenience auto-add win plugin on windows
  if( WIN32 AND NOT Qt5::QWindowsIntegrationPlugin IN_LIST _arg_ADDITIONAL_PLUGINS )
    list( APPEND _arg_ADDITIONAL_PLUGINS Qt5::QWindowsIntegrationPlugin )
  endif()

  list( APPEND qt5_install_libs ${QT5_LINK_LIBS} ${QT5_INSTALL_DEPENDENCIES} )
  foreach( module ${qt5_install_libs} )
    add_custom_command( TARGET ${_arg_TARGET} 
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy
      $<TARGET_FILE:${module}>
      $<TARGET_FILE_DIR:${_arg_TARGET}>
    )
    if( UNIX )
      #the symlink libQt5Core.so.5 -> libQt5Core.so.5.8.0 also needs to be copied
      add_custom_command( TARGET ${_arg_TARGET} 
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_SONAME_FILE:${module}>
        $<TARGET_FILE_DIR:${_arg_TARGET}>
      )    
    endif()
  endforeach()

  if( NOT "${_arg_ADDITIONAL_PLUGINS}" STREQUAL "" )
    set( _qt_known_extra_plugins Qt5::QWindowsIntegrationPlugin;Qt5::QGifPlugin )
    set( _qt_extra_plugin_folders platforms;imageformats )
    foreach( extra_plugin ${_arg_ADDITIONAL_PLUGINS} )
      if()
      else()
        list( FIND _qt_known_extra_plugins ${extra_plugin} _plugin_index )
        list( GET _qt_extra_plugin_folders ${_plugin_index} _plugin_folder )
        add_custom_command( TARGET ${_arg_TARGET} 
          POST_BUILD
          COMMAND ${CMAKE_COMMAND} -E make_directory
          $<TARGET_FILE_DIR:${_arg_TARGET}>/${_plugin_folder}

          COMMAND ${CMAKE_COMMAND} -E copy
          $<TARGET_FILE:${extra_plugin}>
          $<TARGET_FILE_DIR:${_arg_TARGET}>/${_plugin_folder}
        )
      endif()
    endforeach()
  endif()
endfunction()

###################################################################################
## Creates a install targets for used Qt dlls and optional plugins.
##
## usage: setupCopyQtInstall( DESTINATION ${INSTALL_LIB_PATH} ADDITIONAL_PLUGINS Qt5::QGifPlugin )
## Note: Takes care of linked dlls and additional plugins.
##
function( setupCopyQtInstall )
  set( options )
  set( oneValueArgs DESTINATION )
  set( multiValueArgs ADDITIONAL_PLUGINS )
  cmake_parse_arguments( _arg "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )

  if( NOT "DESTINATION" IN_LIST ARGN )
    message( FATAL_ERROR "setupCopyQtInstall: missing param 'DESTINATION'." )
  endif()

  if( "${_arg_DESTINATION}" STREQUAL "" )
    message( FATAL_ERROR "setupCopyQtInstall: no DESTINATION set." )
  endif()

  # convenience auto-add win plugin on windows
  if( WIN32 AND NOT Qt5::QWindowsIntegrationPlugin IN_LIST _arg_ADDITIONAL_PLUGINS )
    list( APPEND _arg_ADDITIONAL_PLUGINS Qt5::QWindowsIntegrationPlugin )
  endif()

  # normal Qt modules
  list( APPEND qt5_install_libs ${QT5_LINK_LIBS} ${QT5_INSTALL_DEPENDENCIES} )
  foreach( module ${qt5_install_libs} )
    if( WIN32 )
      install( FILES $<TARGET_FILE:${module}>
        DESTINATION ${_arg_DESTINATION}
      )
    elseif( UNIX )
      install( FILES $<TARGET_SONAME_FILE:${module}>
        DESTINATION ${_arg_DESTINATION}
      )   
    endif()
  endforeach()

  # additional plugins
  if( NOT "${_arg_ADDITIONAL_PLUGINS}" STREQUAL "" )
    foreach( extra_plugin ${_arg_ADDITIONAL_PLUGINS} )
      if()
      else()
        list( FIND _qt_known_extra_plugins ${extra_plugin} _plugin_index )
        list( GET _qt_extra_plugin_folders ${_plugin_index} _plugin_folder )

        install( FILES
          $<TARGET_FILE:${extra_plugin}>
          DESTINATION ${_arg_DESTINATION}/${_plugin_folder}
        )
      endif()
    endforeach()
  endif()
endfunction()