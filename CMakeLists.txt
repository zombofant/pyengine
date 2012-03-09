cmake_minimum_required(VERSION 2.6)
project(pyUniverse CXX C)

set(CMAKE_CXX_FLAGS "-g -Wall -std=c++0x")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "CMakeModules")

find_package(rt REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Boost COMPONENTS thread regex program_options system signals python REQUIRED)
find_package(PythonInterp REQUIRED)
find_package(PythonLibs REQUIRED)

include_directories(${PROJECT_SOURCE_DIR} ${OpenGL_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${PYTHON_INCLUDE_DIR} client-host/Math client-host/SceneGraph)
set(LIBS ${LIBS} ${OPENGL_gl_LIBRARY} ${Boost_THREAD_LIBRARY} ${Boost_REGEX_LIBRARY} ${Boost_PROGRAM_OPTIONS_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_SIGNALS_LIBRARY} ${Boost_PYTHON_LIBRARY} ${SDL_LIBRARY} ${OPENGL_glu_LIBRARY} ${GLEW_LIBRARY} ${rt_LIBRARY} ${PYTHON_LIBRARY})

add_executable(pyuni-client client-host/pyuni-client.cpp client-host/SceneGraph/Node.cpp client-host/SceneGraph/Spatial.cpp client-host/SceneGraph/SceneGraph.cpp client-host/Math/Vectors.cpp client-host/Math/Matrices.cpp)
target_link_libraries(pyuni-client ${LIBS})
