project(ClientREST)

cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)
if (POLICY CMP0043)
  cmake_policy(SET CMP0043 NEW)
endif()

find_package(Qt5 COMPONENTS Core Network Widgets REQUIRED)

# Auto generate moc files
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

set(ClientREST_SRCS
    mainwindow.cpp
    main.cpp
)

add_executable(client-rest ${ClientREST_SRCS})

target_link_libraries(client-rest
    Qt5::Core
    Qt5::Network
    Qt5::Widgets
)
