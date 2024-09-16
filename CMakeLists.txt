cmake_minimum_required (VERSION 3.14)

#---------------
# Project setup
#---------------
    project (arc)

#----------------------------
# Define include directories
#----------------------------
    set (COMMON_INCLUDES ${PROJECT_SOURCE_DIR}/include)
    include_directories (${COMMON_INCLUDES})

#-------------
# Main target
#-------------
    add_executable (run ${PROJECT_SOURCE_DIR}/src/main.cpp)

#----------------------
# Genious cache target
#----------------------
    add_executable (run_belady  ${PROJECT_SOURCE_DIR}/genious_cache/main.cpp)

#-------------------
# Compilation flags
#-------------------
set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
if (MSVC)
    target_compile_options (run PRIVATE  /W4)
    target_compile_options (run_belady PRIVATE  /W4)
else ()
    target_compile_options (run PRIVATE -O3 -Wall -Wextra -pedantic -Werror)
    target_compile_options (run_belady PRIVATE -O3  -Wall -Wextra -pedantic -Werror)
endif ()

