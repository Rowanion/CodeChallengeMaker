############################################################
## Copyright(c) 2023 Timo Wiesemann. All rights reserved. ##
############################################################

##################################################################################
## debugging function dumps all currently set and known (cached) CMake variables into 
#  	cmake's console window
#  usage: print_known_vars(<FATAL>)
#  if FATAL was given, stop configure after printing this.
function( print_known_vars )
    set( options FATAL )
    set( oneValueArgs)
    set( multiValueArgs )
    cmake_parse_arguments( _arg "${options}" "${oneValueArgs}" "${multiValueArgs}" "${ARGN}" )

    get_cmake_property( _variableNames VARIABLES )
    foreach( _variableName ${_variableNames} )
        message( STATUS "${_variableName}=${${_variableName}}" )
    endforeach()
    if( ${_arg_FATAL} )
        message( FATAL_ERROR "" )
    endif()
endfunction()

########################################################################################
## convenience function to get all cached variable names starting with a certain prefix
########################################################################################
function( getListOfVarsStartingWith _prefix _varResult )
		get_cmake_property(_vars VARIABLES)
		string( REGEX MATCHALL "(^|;)${_prefix}[A-Za-z0-9_]*" _matchedVars "${_vars}" )
		set (_resultVars "")
		foreach( _variable ${_matchedVars} )
				get_property( _type CACHE "${_variable}" PROPERTY TYPE )
				if( NOT "${_type}" STREQUAL "STATIC" )
						list( APPEND _resultVars "${_variable}" )
				endif()
		endforeach()
		set( ${_varResult} ${_resultVars} PARENT_SCOPE )
endfunction()

########################################################################################
## convenience function to get all cached variable names surrounded by prefix and suffix
########################################################################################
function( getListOfVarsBrackedtedBy _prefix _suffix _varResult )
		get_cmake_property( _vars VARIABLES )
		string( REGEX MATCHALL "(^|;)${_prefix}[A-Za-z0-9_]*${_suffix}" _matchedVars "${_vars}" )
		set( _resultVars "" )
		foreach( _variable ${_matchedVars} )
				get_property( _type CACHE "${_variable}" PROPERTY TYPE )
				if( NOT "${_type}" STREQUAL "STATIC" )
						list( APPEND _resultVars "${_variable}" )
				endif()
		endforeach()
		set( ${_varResult} ${_resultVars} PARENT_SCOPE )
endfunction()

################################################################################
## replaces an entry in a list with a new value
#  usage: list_replace_at(LIST list_name INDEX 7 VAL "new value")
function (list_replace_at)
    set( options )
    set( oneValueArgs LIST INDEX VAL )
    set( multiValueArgs )
    cmake_parse_arguments( _arg "${options}" "${oneValueArgs}" "${multiValueArgs}" "${ARGN}" )

    ensure_var_not_empty(VAR ${_arg_INDEX} MSG "[list_replace_at] argument INDEX can't be empty!")
    ensure_var_not_empty(VAR ${_arg_VAL} MSG "[list_replace_at] argument VAL can't be empty!")

    list(INSERT ${_arg_LIST} ${_arg_INDEX} ${_arg_VAL})
    math(EXPR _temp_index "${_arg_INDEX} + 1")
    list(REMOVE_AT ${_arg_LIST} ${_temp_index})

    set(${_arg_LIST} ${${_arg_LIST}} PARENT_SCOPE)
endfunction()

################################################################################
## prints given list variable formatted with indexc and size
#  usage: print_list(LIST list_name)
function(print_list)
    set( options )
    set( oneValueArgs LIST)
    set( multiValueArgs )
    cmake_parse_arguments( _arg "${options}" "${oneValueArgs}" "${multiValueArgs}" "${ARGN}" )
    ensure_var_not_empty(VAR ${_arg_LIST} MSG "[print_list] argument LIST can't be empty!")

    list(LENGTH ${_arg_LIST} _list_legth)
    message(STATUS "\"${_arg_LIST}\" [${_list_legth}]: ")

    set(_list_index 0)
    foreach(_list_entry IN LISTS ${_arg_LIST})
        message(STATUS " ${_list_index}: \t${_list_entry}")
        math(EXPR _list_index "${_list_index} + 1")
    endforeach()

endfunction()

################################################################################
## checks if given variable is empty
#  if it doesen't exist or is empty, issues a fatal message. 
#  ensure_var_not_empty(VAR variable_name <MSG optional_error_message>)
function (ensure_var_not_empty)
    set( options )
    set( oneValueArgs VAR MSG )
    set( multiValueArgs )
    cmake_parse_arguments( _arg "${options}" "${oneValueArgs}" "${multiValueArgs}" "${ARGN}" )

    if(NOT "${_arg_VAR}" STREQUAL "")
        return()
    endif()

    if("${_arg_MSG}" STREQUAL "")
        set(_error_msg "Variable must not be empty!")
    else()
        set(_error_msg ${_arg_MSG})
    endif()
    
    message(FATAL_ERROR ${_error_msg})
endfunction()