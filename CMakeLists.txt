cmake_minimum_required(VERSION 2.6)
project(pyUniverse CXX C)

#find_package(rt REQUIRED)
find_package(OpenGL REQUIRED)
# probably here we should configure the WindowInterface component
# to use, for now just X11 is supported
find_package(X11 COMPONENTS Xinerama REQUIRED)
find_package(Boost COMPONENTS thread regex program_options system signals python REQUIRED)
find_package(PythonInterp REQUIRED)
find_package(PythonLibs REQUIRED)

include_directories(${PROJECT_SOURCE_DIR} ${OpenGL_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${PYTHON_INCLUDE_DIR})

set(LIBS ${LIBS} ${OPENGL_gl_LIBRARY} ${Boost_THREAD_LIBRARY}
    ${Boost_REGEX_LIBRARY} ${Boost_PROGRAM_OPTIONS_LIBRARY}
    ${Boost_SYSTEM_LIBRARY} ${Boost_SIGNALS_LIBRARY} ${Boost_PYTHON_LIBRARY}
    ${SDL_LIBRARY} ${OPENGL_glu_LIBRARY} ${GLEW_LIBRARY} ${rt_LIBRARY}
    ${PYTHON_LIBRARY} ${X11_LIBRARIES} ${X11_Xinerama_LIB})

set(EXEC "pyuni-client")
set(PYUNI_MODULES "pyuni_WindowInterface")
set(CMAKE_CXX_FLAGS "-g -Wall -std=c++0x")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "CMakeModules")

add_subdirectory(cpphost)

add_executable(pyuni-client cpphost/pyuni-client.cpp)
add_dependencies(pyuni-client ${PYUNI_MODULES})
target_link_libraries(${EXEC} ${PYUNI_MODULES} ${LIBS})

