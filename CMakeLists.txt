cmake_minimum_required(VERSION 2.6)

if (CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    # standalone build (e.g. for tests)
    # require some flags
    set(CMAKE_CXX_FLAGS "-g -Werror -Wno-error=unused-parameter -Wno-unused-parameter -Wall -Wextra -std=gnu++11 -msse -msse2 -msse3 -mmmx -O0")
endif (CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)


# macros
    macro(add_sphinx_target TARGET_NAME BUILDER COMMENT_STR)
        add_custom_target(${TARGET_NAME}
        COMMAND sphinx-build -b ${BUILDER} . sphinx/build/${BUILDER}
            WORKING_DIRECTORY docs
            DEPENDS doxygen
            COMMENT ${COMMENT_STR}
        )
    endmacro(add_sphinx_target)

# we have problems with gcc 4.7.0, see
# issue <http://gcc.gnu.org/bugzilla/show_bug.cgi?id=53455>

    if (CMAKE_COMPILER_IS_GNUCXX)
        if (CMAKE_CXX_COMPILER_VERSION VERSION_EQUAL "4.7.0")
            message(FATAL_ERROR "GCC version 4.7.0 is broken with boost::python. See <http://gcc.gnu.org/bugzilla/show_bug.cgi?id=53455>")
        endif (CMAKE_CXX_COMPILER_VERSION VERSION_EQUAL "4.7.0")
    endif (CMAKE_COMPILER_IS_GNUCXX)

# environment
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules")

# -I and -l
    # engine source
    set(PYENGINE_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
    set(LIBS ${LIBS} ${PYENGINE_MODULES})
    set(CONTRIB_MODULES
    #    "contrib_BinPack"
    )
    set(PYENGINE_MODULES
        "pyengine_PythonInterface"
        "pyengine_RenderGraph"
        "pyengine_SceneGraph"
        "pyengine_Resources"
        "pyengine_UI"
        "pyengine_WindowInterface"
        "pyengine_GL"
        "pyengine_VFS"
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
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GLIB glib-2.0)
    # find_package(glib REQUIRED)
    set(PYENGINE_INCLUDE_DIRS ${PYENGINE_INCLUDE_DIRS} ${GLIB_INCLUDE_DIRS})
    set(LIBS ${LIBS} ${GTKMM_LIBRARIES})

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
    set(Python_ADDITIONAL_VERSIONS 2.7 2.6 2.5 2.4)
    find_package(PythonInterp REQUIRED)
    find_package(PythonLibs REQUIRED)
    set(PYENGINE_INCLUDE_DIRS ${PYENGINE_INCLUDE_DIRS} ${PYTHON_INCLUDE_DIR})
    set(LIBS ${LIBS}
        ${PYTHON_LIBRARY}
    )

    # pycairo
    set(PYTHON_EXEC ${PYTHON_EXECUTABLE})
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

    # sigc++
    # we could also use a boosty mechanism for this, but as we also use
    # gtk, it seems reasonable to pick sigc++, which is also used there
    find_package(SIGC++ REQUIRED)
    set(PYENGINE_INCLUDE_DIRS ${PYENGINE_INCLUDE_DIRS}
        ${SIGC++_INCLUDE_DIR}
        ${SIGC++_CONFIG_INCLUDE_DIR})
    set(LIBS ${LIBS} ${SIGC++_LIBRARY})

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


if (CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
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

    set(CATCH_INCLUDE_DIRS "Catch/single_include")
    set(TEST_SOURCES
        "tests/Misc/SetOperations.cpp"
        "tests/Misc/EnumBitset.cpp"
        "tests/Misc/UUID.cpp"
        "tests/Math/Vectors.cpp"
        "tests/Math/Matrices.cpp"
        "tests/VFS/Utils.cpp"
        "tests/UI/test_utils.cpp"
        "tests/UI/CSS/Selectors.cpp"
        "tests/UI/CSS/CSS.cpp"
        "tests/UI/CSS/Shapes.cpp"
        "tests/UI/CSS/Style.cpp"
        "tests/UI/CSS/Rules.cpp"
        "tests/UI/CSS/Theme.cpp"
        "tests/UI/Widgets/WidgetBase.cpp"
        "tests/UI/Widgets/RootWidget.cpp"
        "tests/UI/Widgets/BoxWidget.cpp"
    )


    include_directories(${CATCH_INCLUDE_DIRS})
    add_executable(run_tests tests/main.cpp ${TEST_SOURCES} )
    target_link_libraries(run_tests ${PYENGINE_MODULES} ${CONTRIB_MODULES} ${LIBS})

endif (CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
