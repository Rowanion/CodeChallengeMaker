############################################################
## Copyright(c) 2023 Timo Wiesemann. All rights reserved. ##
############################################################

project(iplugin)

include(FileFunctions)
global_compiler_setup()

##################################################################################
## 3rdParty libs
# Qt stuff

include( UseQt5Modules )
useQt5Modules( true Core )

#########################################################
## The library
add_library(${PROJECT_NAME} OBJECT
   IChallengePlugin.h
)

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Libs")

# this is required so we can link this against .so/.dll AND static
set_property(
   TARGET ${PROJECT_NAME} 
   PROPERTY POSITION_INDEPENDENT_CODE ON
)

target_compile_options( ${PROJECT_NAME}
   PUBLIC
      ${GLOBAL_COMPILE_OPTIONS}
      $<$<CONFIG:Debug>:${GLOBAL_COMPILE_OPTIONS_DEBUG}>
)

target_compile_definitions( ${PROJECT_NAME}
   PUBLIC 
      ${GLOBAL_COMPILE_DEFINITIONS}
)

target_link_libraries(${PROJECT_NAME}
   PUBLIC
      platform
      # Qt5::Core
      ${QT5_LINK_LIBS}
      ${COMMON_LINK_LIBS}
)

target_include_directories(${PROJECT_NAME}
   PUBLIC 
      ${CMAKE_CURRENT_LIST_DIR}
      ${CMAKE_CURRENT_BINARY_DIR}/generated
)
