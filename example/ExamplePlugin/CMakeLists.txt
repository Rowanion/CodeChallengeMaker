############################################################
## Copyright(c) 2023 Timo Wiesemann. All rights reserved. ##
############################################################

project(ExamplePlugin)

include(FileFunctions)
global_compiler_setup()

# configure the resources to add the project name's prefix into it
configure_file( 
   ${CMAKE_CURRENT_SOURCE_DIR}/resources/qt_resources.qrc.in 
   ${CMAKE_CURRENT_BINARY_DIR}/resources/qt_resources.qrc 
   @ONLY
)
configure_file(
   ${CMAKE_CURRENT_SOURCE_DIR}/resources/inputFile.txt 
   ${CMAKE_CURRENT_BINARY_DIR}/resources/inputFile.txt
   COPYONLY
)
configure_file(
   ${CMAKE_CURRENT_SOURCE_DIR}/resources/descriptionFile.txt 
   ${CMAKE_CURRENT_BINARY_DIR}/resources/descriptionFile.txt
   COPYONLY
)

##################################################################################
## 3rdParty libs
# set up / include QT

include( UseQt5Modules )
useQt5Modules( true Core )

set( ${PROJECT_NAME}_RESOURCES 
   ${CMAKE_CURRENT_BINARY_DIR}/resources/qt_resources.qrc
)
#########################################################
## The library

add_cpp_files_to_project(DIRECTORY src SOURCE_GROUP_PREFIX "" RECURSIVE)

add_library(${PROJECT_NAME} SHARED
   ${${PROJECT_NAME}_RESOURCES}
   ${${PROJECT_NAME}_HEADER_FILES}
   ${${PROJECT_NAME}_SOURCE_FILES}
)

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Plugins")

# this is required so we can link this against .so/.dll AND static
set_property(
   TARGET ${PROJECT_NAME} 
   PROPERTY POSITION_INDEPENDENT_CODE ON
)

target_compile_options( ${PROJECT_NAME}
    PUBLIC
      ${GLOBAL_COMPILE_OPTIONS}
    PUBLIC
      $<$<CONFIG:Debug>:${GLOBAL_COMPILE_OPTIONS_DEBUG}>
)

target_compile_definitions( ${PROJECT_NAME}
   PRIVATE
      PLUGIN_NAME="${PROJECT_NAME}"
   PUBLIC 
      ${GLOBAL_COMPILE_DEFINITIONS}
      _SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS=1 # this is due to fmt breaking in current MSVC
      _SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING=1 # this is due to fmt breaking in current MSVC
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
      platform
      iplugin
      common
      ${COMMON_LINK_LIBS}
)

target_include_directories(${PROJECT_NAME} 
    PUBLIC 
      ${CMAKE_CURRENT_LIST_DIR}/src
      ${CMAKE_CURRENT_BINARY_DIR}/generated
)

# to make sure we always build this plugin
add_dependencies(CodeChallengeMaker ${PROJECT_NAME})