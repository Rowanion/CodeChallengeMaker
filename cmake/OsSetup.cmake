############################################################
## Copyright(c) 2023 Timo Wiesemann. All rights reserved. ##
############################################################

include_guard(GLOBAL)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    set(OS_WINDOWS true)
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    set(OS_LINUX true)
else()
    message(FATAL_ERROR "Unsupported Operating System: ${CMAKE_SYSTEM_NAME}")
endif()
