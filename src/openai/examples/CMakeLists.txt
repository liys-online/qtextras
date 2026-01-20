cmake_minimum_required(VERSION 3.16)

project(qopenai_examples VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Network)

# Simple chat example
add_executable(simple_chat
    simple_chat.cpp
)

target_link_libraries(simple_chat PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Network
    openai
)

# Advanced chat example
add_executable(advanced_chat
    advanced_chat.cpp
)

target_link_libraries(advanced_chat PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Network
    openai
)

# Embeddings example
add_executable(embeddings_example
    embeddings_example.cpp
)

target_link_libraries(embeddings_example PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Network
    openai
)

# Models example
add_executable(models_example
    models_example.cpp
)

target_link_libraries(models_example PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Network
    openai
)

add_executable(proxy_example
    proxy_example.cpp)

target_link_libraries(proxy_example PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Network
    openai
)
