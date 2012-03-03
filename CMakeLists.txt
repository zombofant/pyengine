cmake_minimum_required(VERSION 2.6)
project(pyUniverse CXX C)

set(CMAKE_CXX_FLAGS "-g -Wall -std=c++0x")
find_package(Boost COMPONENTS thread regex program_options system signals python REQUIRED)
find_package(PythonInterp)
find_package(PythonLibs)

include_directories(${PROJECT_SOURCE_DIR} ${PYTHON_INCLUDE_DIR})

set(LIBS ${PYTHON_LIBRARY} ${Boost_PYTHON_LIBRARY})

add_executable(pyUniverse host/main.cpp)
target_link_libraries(pyUniverse ${LIBS})
