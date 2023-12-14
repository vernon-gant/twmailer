cmake_minimum_required(VERSION 3.10)

# Project Name
project(MyProject)

# C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wextra -O -pthread")

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/client)
include_directories(${CMAKE_SOURCE_DIR}/server)

# LDAP Include Directory (adjust as needed)
include_directories(/usr/include) # or wherever ldap.h is located

# Recursively find all source files in client and server directories
file(GLOB_RECURSE SOURCES_CLIENT "client/*.cpp")
file(GLOB_RECURSE SOURCES_SERVER "server/*.cpp")

# Create executables for client and server
add_executable(client ${SOURCES_CLIENT})
add_executable(server ${SOURCES_SERVER})

# Specify the output directories for binaries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Set the directory for CMake's output binaries
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)

# Manually specify LDAP libraries
set(LDAP_LIBRARIES ldap lber) # Adjust if needed

# Link the LDAP libraries with your executables
target_link_libraries(client ${LDAP_LIBRARIES})
target_link_libraries(server ${LDAP_LIBRARIES})

# Custom target for cleaning up extra files (not usually necessary)
add_custom_target(clean-all
        COMMAND ${CMAKE_COMMAND} -P clean-all.cmake
)
