cmake_minimum_required(VERSION 3.16)

project(QtExtras VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/src/corelib
    ${CMAKE_CURRENT_SOURCE_DIR}/src/system
)
add_subdirectory(src)
add_subdirectory(tests)
