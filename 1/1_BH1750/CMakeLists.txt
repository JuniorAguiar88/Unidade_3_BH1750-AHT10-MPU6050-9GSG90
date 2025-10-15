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

project(1_BH1750_SERVO_MOTOR C CXX ASM)

add_subdirectory(lib/u8g2pico)

set(PICO_CXX_ENABLE_EXCEPTIONS 1)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add all source files to the project

file(GLOB_RECURSE DRIVER_FILES "drivers/*.c")

# Add executable. Default name is the project name, version 0.1

add_executable(1_BH1750_SERVO_MOTOR 
    1_BH1750_SERVO_MOTOR.c 
    drivers/BH1750/bh1750.c
    drivers/SERVO_MOTOR/servo_motor.c
    )
    
    

pico_set_program_name(1_BH1750_SERVO_MOTOR "1_BH1750_SERVO_MOTOR")
pico_set_program_version(1_BH1750_SERVO_MOTOR "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(1_BH1750_SERVO_MOTOR 0)
pico_enable_stdio_usb(1_BH1750_SERVO_MOTOR 1)

# Add the standard library to the build
target_link_libraries(1_BH1750_SERVO_MOTOR
        pico_stdlib
        hardware_i2c
        hardware_pwm
        u8g2pico)

# Add the standard include files to the build
target_include_directories(1_BH1750_SERVO_MOTOR PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/drivers
)

# Add any user requested libraries
target_link_libraries(1_BH1750_SERVO_MOTOR
        
        )

pico_add_extra_outputs(1_BH1750_SERVO_MOTOR)

