
add_library(pyuni_WindowInterface STATIC
    "Display.cpp"
    "Window.cpp"
    "EventSink.cpp"
    "EventLoop.cpp"
    "X11/EventLoop.cpp"
    "X11/X11Display.cpp"
    "X11/X11Window.cpp"
    )
add_dependencies(pyuni_WindowInterface pyuni_Misc pyuni_IO)
