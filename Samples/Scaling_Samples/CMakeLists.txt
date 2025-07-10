cmake_minimum_required(VERSION 3.2.0 FATAL_ERROR)
set(TARGET_NAME Scaling_Samples)
get_filename_component(ROOT_DIR ../../ ABSOLUTE)
project(Scaling_Samples VERSION 1.0)
add_executable(${TARGET_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/Scaling_App.cpp
    ${ROOT_DIR}/Source/cApiWrapper.cpp
)

if(MSVC)
    set_target_properties(${TARGET_NAME}
        PROPERTIES
            VS_DEBUGGER_COMMAND_ARGUMENTS ""
            VS_DEBUGGER_WORKING_DIRECTORY "$(OutDir)"
    )

    ADD_DEFINITIONS(-DUNICODE)
    ADD_DEFINITIONS(-D_UNICODE)
endif()

include_directories(${ROOT_DIR}/include)
include_directories(${ROOT_DIR}/Samples/inc)