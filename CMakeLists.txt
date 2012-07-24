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

# environment
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules")

# -I and -l
    # engine source
    set(PYENGINE_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/CEngine)
    set(LIBS ${LIBS} ${PYENGINE_MODULES})
    set(CONTRIB_MODULES
        "contrib_BinPack"
    )
    set(PYENGINE_MODULES
        "pyengine_PythonInterface"
        "pyengine_WindowInterface"
        "pyengine_RenderGraph"
        "pyengine_SceneGraph"
        "pyengine_Resources"
        "pyengine_GL"
        "pyengine_IO"
        "pyengine_Math"
        "pyengine_Misc"
    )

    # opengl + glew
    find_package(OpenGL REQUIRED)
    find_package(GLEW REQUIRED)
    set(PYENGINE_INCLUDE_DIRS ${PYENGINE_INCLUDE_DIRS} ${OpenGL_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS})
    set(LIBS ${LIBS} ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY} ${GLEW_LIBRARY})

    # glib
    find_package(glib REQUIRED)
    set(PYENGINE_INCLUDE_DIRS ${PYENGINE_INCLUDE_DIRS} ${glib_INCLUDE_DIRS})
    set(LIBS ${LIBS}
        ${glib_LIBRARIES}
    )

    # pango
    find_package(Pango REQUIRED)
    set(PYENGINE_INCLUDE_DIRS ${PYENGINE_INCLUDE_DIRS} ${Pango_INCLUDE_DIRS})
    set(LIBS ${LIBS}
        ${Pango_LIBRARIES}
    )

    # cairo
    find_package(Cairo REQUIRED)
    set(PYENGINE_INCLUDE_DIRS ${PYENGINE_INCLUDE_DIRS} ${Cairo_INCLUDE_DIRS})
    set(LIBS ${LIBS}
        ${Cairo_LIBRARY}
    )

    # python
    find_package(PythonInterp REQUIRED)
    find_package(PythonLibs REQUIRED)
    set(PYENGINE_INCLUDE_DIRS ${PYENGINE_INCLUDE_DIRS} ${PYTHON_INCLUDE_DIR})
    set(LIBS ${LIBS}
        ${PYTHON_LIBRARY}
    )

    # pycairo
    set(PYTHON_EXEC "/usr/bin/python2")
    find_package(pycairo REQUIRED)
    set(PYENGINE_INCLUDE_DIRS ${PYENGINE_INCLUDE_DIRS} ${pycairo_INCLUDE_DIRS})

    # boost
    find_package(Boost COMPONENTS system python REQUIRED)
    set(PYENGINE_INCLUDE_DIRS ${PYENGINE_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})
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
    set(PYENGINE_INCLUDE_DIRS ${PYENGINE_INCLUDE_DIRS} ${PNG_INCLUDE_DIRS})
    set(PYENGINE_DEFINITIONS ${PNG_DEFINITIONS})

    # freetype
    find_package(Freetype REQUIRED)
    set(LIBS ${LIBS} ${FREETYPE_LIBRARIES})
    set(PYENGINE_INCLUDE_DIRS ${PYENGINE_INCLUDE_DIRS} ${FREETYPE_INCLUDE_DIRS})

# configuration
    configure_file(CEngine/pyengineConfig.template
        CEngine/pyengineConfig.hpp
        ESCAPE_QUOTES)

# targets
    include_directories(${PYENGINE_INCLUDE_DIRS})
    add_definitions(${PYENGINE_DEFINITIONS})

    add_subdirectory(CEngine)
    set_property(DIRECTORY PROPERTY PYENGINE_INCLUDE_DIRS ${PYENGINE_INCLUDE_DIRS})
    set_property(DIRECTORY PROPERTY PYENGINE_DEFINITIONS ${PYENGINE_DEFINITIONS})
    set_property(DIRECTORY PROPERTY PYENGINE_DEPENDENCIES ${PYENGINE_MODULES} ${CONTRIB_MODULES})
    set_property(DIRECTORY PROPERTY PYENGINE_LINK_TARGETS ${PYENGINE_MODULES} ${CONTRIB_MODULES} ${LIBS})
    
    
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
