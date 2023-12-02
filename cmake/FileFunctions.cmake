############################################################
## Copyright(c) 2023 Timo Wiesemann. All rights reserved. ##
############################################################

##################################################################################
## Adds all cpp files in specified "source" directory to variables
## These variables should be added to projects in (add_executable & add_library)
## 
## available variables after call: ${PROJECT_NAME}_HEADER_FILES - contains list of found header files
##                                 ${PROJECT_NAME}_SOURCE_FILES - contains list of found source files
##
## usage: add_cpp_files_to_project( DIRECTORY src SOURCE_GROUP_PREFIX "" RECURSIVE )
## RECURSIVE is optional
##
function( add_cpp_files_to_project )
  set( options RECURSIVE )
  set( oneValueArgs DIRECTORY SOURCE_GROUP_PREFIX )
  set( multiValueArgs )
  cmake_parse_arguments( _arg "${options}" "${oneValueArgs}" "${multiValueArgs}" "${ARGN}" )
  
  if( NOT "SOURCE_GROUP_PREFIX" IN_LIST ARGN )
    set( _arg_SOURCE_GROUP_PREFIX "src" )
  endif()

  if( _arg_RECURSIVE )
    set( _recursive_passthrough "RECURSIVE" )
  else()
    set( recursive_passthrough "" )
  endif()
  add_files_to_project( DIRECTORY ${_arg_DIRECTORY} FILE_EXTENSIONS h hpp SOURCE_GROUP_PREFIX ${_arg_SOURCE_GROUP_PREFIX} OUTPUT_LIST _header_files ${_recursive_passthrough} PROJECT_NAME ${PROJECT_NAME} )
  add_files_to_project( DIRECTORY ${_arg_DIRECTORY} FILE_EXTENSIONS c cpp SOURCE_GROUP_PREFIX ${_arg_SOURCE_GROUP_PREFIX} OUTPUT_LIST _source_files ${_recursive_passthrough} PROJECT_NAME ${PROJECT_NAME} )

  list( APPEND ${PROJECT_NAME}_HEADER_FILES ${_header_files} )
  list( APPEND ${PROJECT_NAME}_SOURCE_FILES ${_source_files} )

  set( ${PROJECT_NAME}_HEADER_FILES ${${PROJECT_NAME}_HEADER_FILES} PARENT_SCOPE )
  set( ${PROJECT_NAME}_SOURCE_FILES ${${PROJECT_NAME}_SOURCE_FILES} PARENT_SCOPE )
endfunction()

##################################################################################
## Adds all windows resource files in specified "resource" directory to variables
## available variables after call: 
##    ${PROJECT_NAME}_WIN_RESOURCE_FILES - contains list of found windows resource files
##
## usage: add_win_resource_files_to_project( DIRECTORY resources SOURCE_GROUP_PREFIX "resources" RECURSIVE )
## RECURSIVE is optional.
##
function( add_win_resource_files_to_project )
  set( options RECURSIVE )
  set( oneValueArgs DIRECTORY SOURCE_GROUP_PREFIX )
  set( multiValueArgs)
  cmake_parse_arguments( _arg "${options}" "${oneValueArgs}" "${multiValueArgs}" "${ARGN}" )

  if( NOT WIN32 )
    return()
  endif()

  if( NOT "SOURCE_GROUP_PREFIX" IN_LIST ARGN )
    set( _arg_SOURCE_GROUP_PREFIX "resources" )
  endif()

  if( _arg_RECURSIVE )
    set( _recursive_passthrough "RECURSIVE" )
  else()
    set( _recursive_passthrough "" )
  endif()

  add_files_to_project( DIRECTORY ${_arg_DIRECTORY} FILE_EXTENSION rc SOURCE_GROUP_PREFIX ${_arg_SOURCE_GROUP_PREFIX} OUTPUT_LIST _resource_files ${_recursive_passthrough} PROJECT_NAME ${PROJECT_NAME} )

  list( APPEND ${PROJECT_NAME}_WIN_RESOURCE_FILES )
  set( ${PROJECT_NAME}_WIN_RESOURCE_FILES ${${PROJECT_NAME}_WIN_RESOURCE_FILES} PARENT_SCOPE )

endfunction()

##################################################################################
## internal function
## Usage: add_files_to_project(DIRECTORY ${_arg_DIRECTORY} FILE_EXTENSIONS h hpp SOURCE_GROUP_PREFIX ${_arg_SOURCE_GROUP_PREFIX} OUTPUT_LIST _header_files ${_recursive_passthrough} PROJECT_NAME ${PROJECT_NAME})

function( add_files_to_project )
  set( options RECURSIVE )
  set( oneValueArgs DIRECTORY SOURCE_GROUP_PREFIX OUTPUT_LIST PROJECT_NAME ) 
  set( multiValueArgs FILE_EXTENSIONS )
  cmake_parse_arguments( _arg "${options}" "${oneValueArgs}" "${multiValueArgs}" "${ARGN}" )

  if( "${_arg_PROJECT_NAME}" STREQUAL "" )
    message( FATAL_ERROR "add_files_to_project: no OUTPUT_LIST set" )
  endif()  

  if( "${_arg_DIRECTORY}" STREQUAL "" )
    message( FATAL_ERROR "add_files_to_project: no DIRECTORY set" )
  endif()

  if( "${_arg_OUTPUT_LIST}" STREQUAL "")
    message( FATAL_ERROR "add_files_to_project: no OUTPUT_LIST set" )
  endif()  

  if( NOT _arg_SOURCE_GROUP_PREFIX )
    set( _source_group_prefix "" )
  else()
    set( _source_group_prefix "PREFIX" )
  endif()

  if( _arg_RECURSIVE )
    set( _recursive_passthrough "RECURSIVE" )
  else()
    set( _recursive_passthrough "" )
  endif()

  get_files_in_subdir_by_extensions( ${_arg_DIRECTORY} OUTPUT_FILE_LIST _file_list FILE_EXTENSIONS ${_arg_FILE_EXTENSIONS} ${_recursive_passthrough} )

  source_group( TREE ${CMAKE_CURRENT_SOURCE_DIR} ${_source_group_prefix} ${_arg_SOURCE_GROUP_PREFIX} FILES ${_file_list} )

  set( ${_arg_OUTPUT_LIST} ${_file_list} PARENT_SCOPE )

  if( _arg_RECURSIVE )
    set( _info_string "recursively" )
  endif()
  message( STATUS "${_arg_PROJECT_NAME}: Adding ${_arg_FILE_EXTENSIONS} ${_info_string} from directory ${CMAKE_CURRENT_SOURCE_DIR}/${_arg_DIRECTORY}" )
endfunction()

##################################################################################
## internal function
## Usage: _create_glob_expressions(DIRECTORY ${_subdir} FILE_EXTENSIONS ${_arg_FILE_EXTENSIONS} OUTPUT_GLOBS _glob_expressions)
function( _create_glob_expressions )
  set( options )
  set( oneValueArgs DIRECTORY OUTPUT_GLOBS )
  set( multiValueArgs FILE_EXTENSIONS )
  cmake_parse_arguments( _arg "${options}" "${oneValueArgs}" "${multiValueArgs}" "${ARGN}" )

   if( "${_arg_DIRECTORY}" STREQUAL "" )
    message( FATAL_ERROR "_create_glob_expressions: no DIRECTORY given" )
  endif()
  
  if( "${_arg_OUTPUT_GLOBS}" STREQUAL "" )
    message(FATAL_ERROR "_create_glob_expiressions: OUTPUT_GLOBS not set")
  endif()
  
  if( "${_arg_FILE_EXTENSIONS}" STREQUAL "" )
    message( FATAL_ERROR "_create_glob_expiressions: FILE_EXTENSIONS not set" )
  endif()

  foreach( _extension ${_arg_FILE_EXTENSIONS} )
    list( APPEND _glob_expressions "${_subdir}/*.${_extension}" ) 
  endforeach()

  set( ${_arg_OUTPUT_GLOBS} ${_glob_expressions} PARENT_SCOPE )
endfunction()

##################################################################################
## internal function
## Usage: get_files_in_subdir_by_extensions(${_arg_DIRECTORY} OUTPUT_FILE_LIST _file_list FILE_EXTENSIONS ${_arg_FILE_EXTENSIONS} ${_recursive_passthrough})
function( get_files_in_subdir_by_extensions _arg_subdir )
  set( options RECURSIVE )
  set( oneValueArgs OUTPUT_FILE_LIST )
  set( multiValueArgs FILE_EXTENSIONS )
  cmake_parse_arguments( PARSE_ARGV 1 _arg "${options}" "${oneValueArgs}" "${multiValueArgs}" )

  if( "${_arg_subdir}" STREQUAL "" )
    message( FATAL_ERROR "get_files_in_subdir_by_extensions: no subdir given" )
  endif()
  
  if( "${_arg_OUTPUT_FILE_LIST}" STREQUAL "" )
    message( FATAL_ERROR "get_files_in_subdir_by_extensions: OUTPUT_FILE_LIST not set" )
  endif()
  
  if( "${_arg_FILE_EXTENSIONS}" STREQUAL "")
    message(  FATAL_ERROR "get_files_in_subdir_by_extensions: FILE_EXTENSIONS not set" )
  endif()

  if( IS_ABSOLUTE ${_arg_subdir} )
    file( RELATIVE_PATH _subdir ${CMAKE_CURRENT_SOURCE_DIR} ${_arg_subdir} )
  else()
    set( _subdir ${_arg_subdir} )
  endif()

  if( NOT IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${_subdir} )
    message( FATAL_ERROR "get_files_in_subdir_by_extensions:  ${CMAKE_CURRENT_SOURCE_DIR}/${_subdir} does not exist" )
  endif()

  _create_glob_expressions( DIRECTORY ${_subdir} 
    FILE_EXTENSIONS ${_arg_FILE_EXTENSIONS} 
    OUTPUT_GLOBS _glob_expressions)

  if(_arg_RECURSIVE)
    set(_glob_type "GLOB_RECURSE")
  else()
    set(_glob_type "GLOB")
  endif()

  file( ${_glob_type} _files
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    CONFIGURE_DEPENDS
    ${_glob_expressions}
  )

  set( ${_arg_OUTPUT_FILE_LIST} ${_files} PARENT_SCOPE )
endfunction()

##################################################################################
## Copies all given files to binary dir on PostBuild
## usage: setup_copy_to_bin_post_build( TARGET targetForPostBuild FILES path/to/files/to/copy ... )
##
function( setup_copy_to_bin_post_build )
  set( options )
  set( oneValueArgs TARGET DESTINATION )
  set( multiValueArgs FILES )
  cmake_parse_arguments( _arg "${options}" "${oneValueArgs}" "${multiValueArgs}" "${ARGN}" )

  if( NOT TARGET ${_arg_TARGET} )
    message( FATAL_ERROR "setup_copy_to_bin_post_build: ${_arg_TARGET} is not a valid target." )
  endif()

  if( NOT _arg_FILES )
    message( FATAL_ERROR "setup_copy_to_bin_post_build: FILES can't be empty!" )
  endif()

  if( _arg_DESTINATION )
     add_custom_command( TARGET ${_arg_TARGET} 
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:${_arg_TARGET}>/${_arg_DESTINATION}
    )
  endif()

  # make sure the dir exists and make if NOT
  foreach( file ${_arg_FILES} )
    if( NOT EXISTS ${file} )
      message( FATAL_ERROR "setup_copy_to_bin_post_build: file '${file}' does not exist and can't be copied!" )
    endif()
    add_custom_command( TARGET ${_arg_TARGET} 
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
      ${file}
      $<TARGET_FILE_DIR:${_arg_TARGET}>/${_arg_DESTINATION}
    )
  endforeach()  
endfunction()

##################################################################################
## Creates and empty file or with CONTENT next to TARGET's binary after compilation.
## usage: mk_file_at_target_post_build( TARGET targetForPostBuild FILE_NAME file_to_be_written <CONTENT file_contents> )
##
function(mk_file_at_target_post_build)
  set( options )
  set( oneValueArgs TARGET FILE_NAME CONTENT )
  set( multiValueArgs )
  cmake_parse_arguments( _arg "${options}" "${oneValueArgs}" "${multiValueArgs}" "${ARGN}" )

  if(${_arg_TARGET} STREQUAL "")
    message(FATAL_ERROR "mk_file_at_target_post_build: \"TARGET\" must not be empty!")
  elseif(NOT TARGET ${_arg_TARGET})
    message(FATAL_ERROR "mk_file_at_target_post_build: \"TARGET\" must be a valid target")
  endif()

  if(${_arg_FILE_NAME} STREQUAL "")
    message(FATAL_ERROR "mk_file_at_target_post_build: \"FILE_NAME\" must not be empty!")
  endif()

  add_custom_command( TARGET ${_arg_TARGET} 
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E echo_append ${_arg_CONTENT} > $<TARGET_FILE_DIR:${_arg_TARGET}>/${_arg_FILE_NAME}
    )
endfunction()