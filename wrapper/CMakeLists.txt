cmake_minimum_required(VERSION 3.9)

set(project "IGCL_Wrapper")

set(all_file
  "cApiWrapper.cpp"
  ${CMAKE_SOURCE_DIR}/../includes/igcl_api.h
 )

 set(BUILD_SHARED_LIBS true)
 set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS true)

add_library(${project} ${all_file})

