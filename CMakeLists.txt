# The top-level CMakeLists.txt contains the overall project setup, find_package and add_subdirectory calls:

cmake_minimum_required(VERSION 3.16)
project(MLPProject VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(QT_DEBUG_PLUGINS true)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_subdirectory(src/app)