############################################################
## Copyright(c) 2023 Timo Wiesemann. All rights reserved. ##
############################################################

project(common)

include(FileFunctions)
global_compiler_setup()

##################################################################################
## 3rdParty libs
# include(3rdPartySetup)
# setup3rdPartyLibsCommon()

#########################################################
## The library

add_cpp_files_to_project(DIRECTORY src SOURCE_GROUP_PREFIX "" RECURSIVE)

add_library(${PROJECT_NAME} STATIC
   ${${PROJECT_NAME}_HEADER_FILES}
   ${${PROJECT_NAME}_SOURCE_FILES}
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
    PUBLIC
      $<$<CONFIG:Debug>:${GLOBAL_COMPILE_OPTIONS_DEBUG}>
)

target_compile_definitions( ${PROJECT_NAME}
    PUBLIC 
      ${GLOBAL_COMPILE_DEFINITIONS}
      _SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS=1 # this is due to fmt breaking in current MSVC
      _SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING=1 # this is due to fmt breaking in current MSVC
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
      platform
      ${COMMON_LINK_LIBS}
)

target_include_directories(${PROJECT_NAME} 
    PUBLIC 
      ${CMAKE_CURRENT_LIST_DIR}/src
      ${CMAKE_CURRENT_BINARY_DIR}/generated
)
