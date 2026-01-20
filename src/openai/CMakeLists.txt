cmake_minimum_required(VERSION 3.16)

project(openai VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Network)

add_library(openai SHARED
  openai_global.h
  qopenaitypes.h
  qopenaiexceptions.h
  qopenairesponse.h
  qopenairesponse.cpp
  qopenaibaseclient.h
  qopenaibaseclient.cpp
  qopenaichat.h
  qopenaichat.cpp
  qopenairesources.h
  qopenairesources.cpp
  qopenai.h
  qopenai.cpp
)

target_link_libraries(openai PRIVATE
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Network
)

target_compile_definitions(openai PRIVATE OPENAI_LIBRARY)

# Install targets
install(TARGETS openai
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

# Install headers
install(FILES
    openai_global.h
    qopenaitypes.h
    qopenaiexceptions.h
    qopenairesponse.h
    qopenaibaseclient.h
    qopenaichat.h
    qopenairesources.h
    qopenai.h
    DESTINATION include/qopenai
)
add_subdirectory(examples)
