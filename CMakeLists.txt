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

#---------
# Testing
#---------
    # Google test is a git submodule for the project, and it is also cmake-based.
    add_subdirectory (googletest)

    enable_testing ()

    # Include the gtest library. gtest_SOURCE_DIR is available due to
    # 'project(gtest)' above.
    include_directories (${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

    # Testing files.
    file (GLOB TEST_SRC_FILES ${PROJECT_SOURCE_DIR}/testing/*.cpp)

    # Unit tests target.
    add_executable (run_unit_tests ${TEST_SRC_FILES})

    # Standard linking to gtest stuff.
    target_link_libraries (run_unit_tests gtest gtest_main)

    # This is so you can do 'make test' to see all your tests run, instead of
    # manually running the executable run_unit_tests to see those specific tests.
    add_test (UnitTests run_unit_tests)

