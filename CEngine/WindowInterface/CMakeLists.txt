
add_library(pyengine_WindowInterface STATIC
    "Display.cpp"
    "Window.cpp"
    "EventSink.cpp"
    "EventLoop.cpp"
    "X11/EventLoop.cpp"
    "X11/X11Display.cpp"
    "X11/X11Window.cpp"
    )
add_dependencies(pyengine_WindowInterface pyengine_IO)
