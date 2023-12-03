############################################################
## Copyright(c) 2023 Timo Wiesemann. All rights reserved. ##
############################################################

cmake_minimum_required( VERSION 3.15.0 FATAL_ERROR )

#Use <PackagenName>_ROOT variables
cmake_policy( SET CMP0074 NEW )
#in CMake 3.10 AUTOMOC and AUTOUIC will process GENERATED files. To get rid of the warning disable that policy 
cmake_policy( SET CMP0071 NEW )

#######################################################################
## 3rdParty Libs

# Qt stuff
if( UNIX )
  list( APPEND ${PROJECT_NAME}_QT_PLUGINS 
    Qt5::QXcbGlxIntegrationPlugin 
    Qt5::QXcbIntegrationPlugin
  )
endif()

include( UseQt5Modules )
useQt5Modules( true Core Gui Widgets )

if( WIN32 )
  useQt5Modules( true WinExtras )
elseif( UNIX )
  #the xcb platform plugin has a dependency for DBus, there is no need to link against it, but it has to be installed
  useQt5Modules( false DBus ) 
  list( APPEND QT5_INSTALL_DEPENDENCIES Qt5::DBus )
endif()

set( ${PROJECT_NAME}_RESOURCES 
  ${CMAKE_CURRENT_SOURCE_DIR}/resources/qt_resources.qrc
)

#######################################################################
## This project's sources
include( FileFunctions )
add_cpp_files_to_project( DIRECTORY src SOURCE_GROUP_PREFIX "" RECURSIVE )

# the application icon
if( WIN32 )
  set( ${PROJECT_NAME}_WIN_RESOURCES ${CMAKE_CURRENT_SOURCE_DIR}/resources/win_icon.rc )
endif()

#######################################################################
## create/setup the binary
add_executable( ${PROJECT_NAME} 
  ${${PROJECT_NAME}_SOURCE_FILES}
  ${${PROJECT_NAME}_HEADER_FILES}
  ${${PROJECT_NAME}_UIS_H}
  ${${PROJECT_NAME}_RESOURCES}
  ${${PROJECT_NAME}_WIN_RESOURCES}
)
source_group( "Resources Files" FILES ${${PROJECT_NAME}_RESOURCES} ${${PROJECT_NAME}_WIN_RESOURCES} )

# decide if its a win32 binary
if( WIN32 )
  set_target_properties( ${PROJECT_NAME} PROPERTIES 
    WIN32_EXECUTABLE true
  )
else()
  set_target_properties( ${PROJECT_NAME} PROPERTIES 
    WIN32_EXECUTABLE false
  )
endif()

target_include_directories( ${PROJECT_NAME}
  PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/ui>
    ${CMAKE_CURRENT_BINARY_DIR}/generated
    ${PROJECT_BINARY_DIR}/src/
)

target_link_libraries( ${PROJECT_NAME}  
  PRIVATE
   common
   ${QT5_LINK_LIBS}
   iplugin
)

target_compile_options( ${PROJECT_NAME}
  PUBLIC
    ${GLOBAL_COMPILE_OPTIONS}
  PUBLIC
    $<$<CONFIG:Debug>:${GLOBAL_COMPILE_OPTIONS_DEBUG}>
)

target_compile_definitions( ${PROJECT_NAME}
  PUBLIC 
  ${GLOBAL_COMPILE_DEFINITIONS}
  _SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS=1 # this is due to fmt breaking in current MSVC
  _SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING=1 # this is due to fmt breaking in current MSVC
)

# copy required dlls to build folder
copyQtPostBuild( TARGET ${PROJECT_NAME} ADDITIONAL_PLUGINS ${${PROJECT_NAME}_QT_PLUGINS} )

###############################################################################
## install directives

if( WIN32)
  set( INSTALL_LIB_PATH "./" )
  set( INSTALL_BIN_PATH "./" )
else()
  set( INSTALL_LIB_PATH "lib" )
  set( INSTALL_BIN_PATH "bin" )
endif()

install( TARGETS ${PROJECT_NAME} 
  DESTINATION ${INSTALL_BIN_PATH}
)

#set rpath on linux, so the copied Qt libs are found
if( UNIX )
  set_target_properties( ${PROJECT_NAME} PROPERTIES INSTALL_RPATH $ORIGIN/../lib  )
endif()

if( UNIX )
  
  install( FILES
    #libQt5Core does link against icu i18n libs that are located ot the installation folder, we need to copy them as well
    $<TARGET_SONAME_FILE_DIR:Qt5::Core>/libicuuc.so.56.1 #lib 
    $<TARGET_SONAME_FILE_DIR:Qt5::Core>/libicuuc.so.56   #symlimk

    $<TARGET_SONAME_FILE_DIR:Qt5::Core>/libicui18n.so.56.1 #lib 
    $<TARGET_SONAME_FILE_DIR:Qt5::Core>/libicui18n.so.56   #symlimk

    $<TARGET_SONAME_FILE_DIR:Qt5::Core>/libicudata.so.56.1 #lib 
    $<TARGET_SONAME_FILE_DIR:Qt5::Core>/libicudata.so.56   #symlimk

    $<TARGET_SONAME_FILE_DIR:Qt5::Core>/libicudata.so.56.1 #lib 
    $<TARGET_SONAME_FILE_DIR:Qt5::Core>/libicudata.so.56   #symlimk

    #the xcb plaformplugin comes with a private lib, that cannot be installed via targets (https://bugreports.qt.io/browse/QTBUG-43202)
    $<TARGET_SONAME_FILE_DIR:Qt5::Core>/libQt5XcbQpa.so.${QT5_VERSION} #lib
    $<TARGET_SONAME_FILE_DIR:Qt5::Core>/libQt5XcbQpa.so.${QT5_VERSION_MAJOR}.${QT5_VERSION_MINOR} #symlink
    $<TARGET_SONAME_FILE_DIR:Qt5::Core>/libQt5XcbQpa.so.${QT5_VERSION_MAJOR} #symlink 
    DESTINATION ${INSTALL_LIB_PATH}
  )
endif()

# Qt
setupCopyQtInstall( 
  DESTINATION ${INSTALL_LIB_PATH} 
  ADDITIONAL_PLUGINS ${${PROJECT_NAME}_QT_PLUGINS} 
)

if( WIN32 )
  set(CMAKE_INSTALL_DEBUG_LIBRARIES TRUE)
  set(CMAKE_INSTALL_DEBUG_LIBRARIES_ONLY TRUE)
  set(CMAKE_INSTALL_UCRT_LIBRARIES TRUE)
  #skip the install, we only want the debug runtime for the debug configuration and not for the release ones.
  set(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_SKIP TRUE) 
  include(InstallRequiredSystemLibraries)

  install(PROGRAMS ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS}
    DESTINATION ${INSTALL_BIN_PATH} 
    CONFIGURATIONS Debug
  )
endif()
