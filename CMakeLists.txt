cmake_minimum_required(VERSION 3.20)
set(WIN32_PROJECT_NAME TaskbarRounder)
project(${WIN32_PROJECT_NAME})

set(CMAKE_CXX_STANDARD 20)

add_compile_definitions(_UNICODE UNICODE)
add_link_options(
        /SUBSYSTEM:WINDOWS
        /DYNAMICBASE
        /MACHINE:X64
        Dwmapi.lib
)

add_executable(
        ${WIN32_PROJECT_NAME}
        main.cpp
)

set_target_properties(${WIN32_PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)
