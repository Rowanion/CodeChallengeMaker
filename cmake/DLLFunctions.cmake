############################################################
## Copyright(c) 2023 Timo Wiesemann. All rights reserved. ##
############################################################

#######################################################################
## usage: gather_shared_dependencies( SteamDeckup )
##  recursively iterates over all of target's LINK_INTERFACE
##  dependencies and appens all of those with DLLs to a global
##  variable ${PROJECT_NAME}_SHARED_DEPENDENCIES.
##  This variable is used later to copy dlls for <CONFIG>.
#######################################################################
function( gather_shared_dependencies TARGET_NAME )
  if( TARGET ${TARGET_NAME} )
    # collect all direct and indirect dependencies from source target.
    set( LINK_LIBRARIES_FINAL )
    get_target_property( _link_interface_libraries ${TARGET_NAME} LINK_LIBRARIES )
    list( REMOVE_DUPLICATES _link_interface_libraries )
    
    # recursively collect all dependencies
    while( _link_interface_libraries )
      list( GET _link_interface_libraries 0 _link_target )
      list( REMOVE_AT _link_interface_libraries 0 )

      if( TARGET ${_link_target} AND NOT ${_link_target} IN_LIST LINK_LIBRARIES_FINAL )

        list( APPEND LINK_LIBRARIES_FINAL ${_link_target} )
        get_target_property( _additional_targets ${_link_target} INTERFACE_LINK_LIBRARIES )
        if( _additional_targets )
          list( APPEND _link_interface_libraries ${_additional_targets} )
          list( REMOVE_DUPLICATES _link_interface_libraries )
        endif()
      endif()

    endwhile()
    list( REMOVE_DUPLICATES LINK_LIBRARIES_FINAL )

    # check if target is a dll itself
    get_target_property( target_type ${TARGET_NAME} TYPE )
    if( target_type STREQUAL "SHARED_LIBRARY" )
      list( APPEND LINK_LIBRARIES_FINAL ${TARGET_NAME} )
    endif()

    # finally gather all shared dependencies in one central place
    foreach( _link_target ${LINK_LIBRARIES_FINAL} )
      if( TARGET ${_link_target} )
        get_target_property( target_type ${_link_target} TYPE )
        if( target_type STREQUAL "SHARED_LIBRARY" )
          list( APPEND ${PROJECT_NAME}_SHARED_DEPENDENCIES ${_link_target} )
        endif()
      endif()
    endforeach()

    list( REMOVE_DUPLICATES ${PROJECT_NAME}_SHARED_DEPENDENCIES )
    set( ${PROJECT_NAME}_SHARED_DEPENDENCIES ${${PROJECT_NAME}_SHARED_DEPENDENCIES} PARENT_SCOPE )
  else()
      message( WARNING "${TARGET_NAME} is not a target!" )
  endif()
endfunction()

function( setup_copy_dll_post_build )
  set( options )
  set( oneValueArgs TARGET )
  set( multiValueArgs )
  cmake_parse_arguments( _arg "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )

  if( TARGET ${_arg_TARGET} )
    foreach( _shared_lib ${${PROJECT_NAME}_SHARED_DEPENDENCIES} )
      message("copying: ${_shared_lib}")
      add_custom_command( TARGET ${_arg_TARGET} 
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:${_shared_lib}>
        $<TARGET_FILE_DIR:${_arg_TARGET}>
      )
    endforeach()
  else()
    message( WARNING "setup_copy_dll_post_build: ${_arg_TARGET} is not a valid target." )
  endif()
endfunction()

function( setup_copy_dll_install )
  set( options )
  set( oneValueArgs DESTINATION )
  set( multiValueArgs )
  cmake_parse_arguments( _arg "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )

  if( NOT "DESTINATION" IN_LIST ARGN )
    message( FATAL_ERROR "setup_copy_dll_install: missing param 'DESTINATION'." )
  endif()

  if( "${_arg_DESTINATION}" STREQUAL "" )
    message( FATAL_ERROR "setup_copy_dll_install: no DESTINATION set." )
  endif()

  foreach( _shared_lib ${${PROJECT_NAME}_SHARED_DEPENDENCIES} )
    install( FILES 
      $<TARGET_FILE:${_shared_lib}>
      DESTINATION ${_arg_DESTINATION} 
    )
  endforeach()
endfunction()