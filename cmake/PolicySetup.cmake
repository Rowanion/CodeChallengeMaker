############################################################
## Copyright(c) 2023 Timo Wiesemann. All rights reserved. ##
############################################################

include_guard(GLOBAL)


# for finding, use <PackagenName>_ROOT variables
cmake_policy(SET CMP0074 NEW)

# use the same bitness as target and BOTH bitnesses as default for find_ commands
cmake_policy(SET CMP0134 NEW)

# generate an error if an imported target has its location not set
cmake_policy(SET CMP0111 NEW)

# included files do automatic policy push/pop
cmake_policy(SET CMP0011 NEW)

# no longer implicitly dereference variables or interpret keywords in an if() command argument when it is a quoted argument or a bracket argument.
cmake_policy(SET CMP0054 NEW)

# for compatibility with versions of CMake that did not have the BYPRODUCTS option, CMake is generating phony rules for such files to convince 'ninja' to build.
cmake_policy(SET CMP0058 NEW)

# disallow source files with missing file extension
cmake_policy(SET CMP0115 NEW)