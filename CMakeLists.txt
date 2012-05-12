cmake_minimum_required(VERSION 2.6)

# macros
    macro(add_sphinx_target TARGET_NAME BUILDER COMMENT_STR)
        add_custom_target(${TARGET_NAME}
        COMMAND sphinx-build -b ${BUILDER} . sphinx/build/${BUILDER}
            WORKING_DIRECTORY docs
            DEPENDS doxygen
            COMMENT ${COMMENT_STR}
        )
    endmacro(add_sphinx_target)

# project setup
    project(pyUniverse CXX C)
    set(EXEC "pyuni-client")
    set(CMAKE_CXX_FLAGS "-g -Wall -std=c++0x")
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "CMakeModules")

# -I and -l
    # pyUniverse
    include_directories(${PROJECT_SOURCE_DIR}/cpphost)
    set(LIBS ${LIBS} ${PYUNI_MODULES})
    set(CONTRIB_MODULES
        "contrib_BinPack"
    )
    set(PYUNI_MODULES
        "pyuni_PythonInterface"
        "pyuni_WindowInterface"
        "pyuni_RenderGraph"
        "pyuni_SceneGraph"
        "pyuni_Resources"
        "pyuni_GL"
        "pyuni_IO"
        "pyuni_Math"
        "pyuni_Misc"
    )

    # opengl + glew
    find_package(OpenGL REQUIRED)
    find_package(GLEW REQUIRED)
    include_directories(${OpenGL_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS})
    set(LIBS ${LIBS} ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY} ${GLEW_LIBRARY})

    # glib
    find_package(glib REQUIRED)
    include_directories(${glib_INCLUDE_DIRS})
    set(LIBS ${LIBS}
        ${glib_LIBRARIES}
    )

    # pango
    find_package(Pango REQUIRED)
    include_directories(${Pango_INCLUDE_DIRS})
    set(LIBS ${LIBS}
        ${Pango_LIBRARIES}
    )

    # cairo
    find_package(Cairo REQUIRED)
    include_directories(${Cairo_INCLUDE_DIRS})
    set(LIBS ${LIBS}
        ${Cairo_LIBRARY}
    )

    # python
    find_package(PythonInterp REQUIRED)
    find_package(PythonLibs REQUIRED)
    include_directories(${PYTHON_INCLUDE_DIR})
    set(LIBS ${LIBS}
        ${PYTHON_LIBRARY}
    )

    # pycairo
    set(PYTHON_EXEC "/usr/bin/python2")
    find_package(pycairo REQUIRED)
    include_directories(${pycairo_INCLUDE_DIRS})

    # boost
    find_package(Boost COMPONENTS system python REQUIRED)
    include_directories(${Boost_INCLUDE_DIRS})
    set(LIBS ${LIBS}
    #    ${Boost_THREAD_LIBRARY}
    #    ${Boost_REGEX_LIBRARY}
    #    ${Boost_PROGRAM_OPTIONS_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
    #    ${Boost_SIGNALS_LIBRARY}
        ${Boost_PYTHON_LIBRARY}
    )

    # PLATFORM: linux
    find_package(rt REQUIRED)
    set(LIBS ${LIBS}
        ${rt_LIBRARY}
    )

    # PLATFORM: X11

    # probably here we should configure the WindowInterface component
    # to use, for now just X11 is supported
    find_package(X11 COMPONENTS Xinerama REQUIRED)
    set(LIBS ${LIBS}
        ${X11_LIBRARIES} ${X11_Xinerama_LIB}
    )

    # libpng
    find_package(PNG REQUIRED)
    set(LIBS ${LIBS} ${PNG_LIBRARIES})
    include_directories(${PNG_INCLUDE_DIRS})
    add_definitions(${PNG_DEFINITIONS})

    # freetype
    find_package(Freetype REQUIRED)
    set(LIBS ${LIBS} ${FREETYPE_LIBRARIES})
    include_directories(${FREETYPE_INCLUDE_DIRS})

# configuration
    configure_file(cpphost/pyuniConfig.template
        cpphost/pyuniConfig.hpp
        ESCAPE_QUOTES)

# targets
    add_subdirectory(cpphost)

    add_executable(pyuni-client cpphost/pyuni-client.cpp)
    add_dependencies(pyuni-client ${PYUNI_MODULES} ${CONTRIB_MODULES})
    target_link_libraries(${EXEC} ${PYUNI_MODULES} ${CONTRIB_MODULES} ${LIBS})

# documentation targets
    add_custom_target(doxygen
        COMMAND doxygen docs/doxygen.conf
        COMMENT "Build doxygen xml files used by sphinx/breathe."
    )

    add_sphinx_target(docs-html
        html
        "Build html documentation"
    )

    add_sphinx_target(docs-pdf
        latexpdf
        "Build pdf documentation"
    )

    add_sphinx_target(docs-man
        man
        "Build manpages"
    )

    add_custom_target(docs
        DEPENDS docs-html)

    add_custom_target(clean-docs
        COMMAND ./clean.sh
        WORKING_DIRECTORY docs/sphinx
        COMMENT "Clean all generated documentation"
    )
