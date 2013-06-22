add_library(pyengine_UI
  "Selectors.cpp"
  "CSS.cpp"
  "Shapes.cpp"
  "Fill.cpp"
  "Border.cpp"
  "Style.cpp"
  "BaseRules.cpp"
  "Rules.cpp"
  "Theme.cpp"
  "WidgetBase.cpp"
  "DragController.cpp"
  "RootWidget.cpp"
)
add_dependencies(pyengine_UI pyengine_GL)
