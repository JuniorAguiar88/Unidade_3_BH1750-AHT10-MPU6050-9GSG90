# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.2.0)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.2.0-a4)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(2_AHT10 C CXX ASM)

#Add U8G2PICO lib folder
add_subdirectory(lib/u8g2pico)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add all source files to the project
file(GLOB_RECURSE DRIVER_FILES "drivers/AHT10*.c")

# Add executable. Default name is the project name, version 0.1

add_executable(2_AHT10 2_AHT10.c 
    drivers/AHT10/aht10.c
    )

pico_set_program_name(2_AHT10 "2_AHT10")
pico_set_program_version(2_AHT10 "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(2_AHT10 0)
pico_enable_stdio_usb(2_AHT10 1)

# Add the standard library to the build
target_link_libraries(2_AHT10
        pico_stdlib
        hardware_i2c
        u8g2pico)

# Add the standard include files to the build
target_include_directories(2_AHT10 PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/drivers
)

# Add any user requested libraries
target_link_libraries(2_AHT10 
        
        )

pico_add_extra_outputs(2_AHT10)

