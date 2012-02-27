# This package contains copies of pyglet modules which are not dependent
# on available opengl. It is used to import certain constants even if
# pyglet is not available
"""
Usage:
    from Engine.pygletHeadless import $pygletModule

    where $pygletModule should be one of the supported modules.

Supported modules:
    pyglet.window.mouse => mouse
    pyglet.window.key => key
"""

try:
    import pyglet
    import pyglet.window.mouse as mouse
    import pyglet.window.key as key
except ImportError:
    import _key as key
    import _mouse as mouse
    
