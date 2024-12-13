cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME WheelLeg-test)

# Include toolchain file
include("cmake/gcc-arm-none-eabi.cmake")

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Enable CMake support for ASM and C languages and C++
enable_language(C ASM CXX)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths
    {CMAKE_SOURCE_DIR}/Core/Inc
)

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user sources here
    ${CMAKE_SOURCE_DIR}/Core/Src/WheelLeg/tasks/tasks.cpp
    ${CMAKE_SOURCE_DIR}/Core/Src/WheelLeg/tasks/TaskLed.cpp
    ${CMAKE_SOURCE_DIR}/Core/Src/WheelLeg/tasks/TaskWheel.cpp
    ${CMAKE_SOURCE_DIR}/Core/Src/WheelLeg/tasks/TaskReadDbus.cpp
    ${CMAKE_SOURCE_DIR}/Core/Src/WheelLeg/tasks/TaskJoint.cpp

    ${CMAKE_SOURCE_DIR}/Core/Src/PHOENIX/BaseControl/Connectivity/Connectivity.cpp
    ${CMAKE_SOURCE_DIR}/Core/Src/PHOENIX/BaseControl/Connectivity/CAN/CAN.cpp
    ${CMAKE_SOURCE_DIR}/Core/Src/PHOENIX/BaseControl/Connectivity/UART/UART.cpp
    ${CMAKE_SOURCE_DIR}/Core/Src/PHOENIX/BaseControl/Connectivity/UART/DBUS.cpp
    
    ${CMAKE_SOURCE_DIR}/Core/Src/PHOENIX/BaseControl/Motor/Motor.cpp
    ${CMAKE_SOURCE_DIR}/Core/Src/PHOENIX/BaseControl/Motor/RM3508.cpp
    ${CMAKE_SOURCE_DIR}/Core/Src/PHOENIX/BaseControl/Motor/DM4310.cpp
    
    ${CMAKE_SOURCE_DIR}/Core/Src/PHOENIX/BaseControl/Controller/pidController.cpp
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined include paths
)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined symbols
)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx
    -std=c++11
    # Add user defined libraries
)

add_custom_command(OUTPUT flash
    COMMAND JLinkExe -if SWD -CommanderScript ${CMAKE_SOURCE_DIR}/.JLinkScripts
)
add_custom_target(Flash DEPENDS flash)