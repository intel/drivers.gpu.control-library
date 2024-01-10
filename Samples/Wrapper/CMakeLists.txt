cmake_minimum_required(VERSION 3.9)

get_filename_component(ROOT_DIR ../../ ABSOLUTE)
set(project "IGCL_Wrapper")

set(all_file
    "Wrapper.cpp"
    ${ROOT_DIR}/wrapper/cApiWrapper.cpp
)

set(BUILD_SHARED_LIBS true)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS true)

include_directories(${ROOT_DIR}/includes)
include_directories(${ROOT_DIR}/Samples/inc)

add_library(${project} ${all_file})