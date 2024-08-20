cmake_minimum_required(VERSION 3.10)
project(hayflags)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Default to building a static library
option(SHARED_flags "Build hayflags as a shared library" OFF)

# Define the library name
set(LIBRARY_NAME "hayflags")

# Source files for the library
set(SOURCES
    src/flags.c
    # Add additional source files here if needed
)

# Include directories
include_directories(include)

# Create the library
if(SHARED_flags)
    add_library(${LIBRARY_NAME} SHARED ${SOURCES})
else()
    add_library(${LIBRARY_NAME} STATIC ${SOURCES})
endif()

# Set the version for shared libraries
if(SHARED_flags)
    set_target_properties(${LIBRARY_NAME} PROPERTIES
        VERSION 1.0
        SOVERSION 1
    )
endif()

# Install targets
install(TARGETS ${LIBRARY_NAME}
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

# Install headers
install(DIRECTORY include/ DESTINATION include)

# Set library version
set_target_properties(${LIBRARY_NAME} PROPERTIES VERSION 1.0 SOVERSION 1)

# Enable testing
enable_testing()

# Define the test executable
add_executable(test_flags tests/test_flags.c)

# Link the test executable with the library
target_link_libraries(test_flags PRIVATE ${LIBRARY_NAME})

# Define a test case
add_test(NAME test_flags COMMAND test_flags)

# Optionally, add additional configuration here
