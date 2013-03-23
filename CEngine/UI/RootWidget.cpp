#include "RootWidget.hpp"

namespace PyEngine {

/* PyEngine::RootWidget */

RootWidget::RootWidget():
    AbstractWidget(),
    WidgetContainer(),
    EventSink(),
    std::enable_shared_from_this<RootWidget>()
{

}

RootWidget::~RootWidget()
{

}

RootPtr RootWidget::get_root()
{
    return this->std::enable_shared_from_this<RootWidget>::shared_from_this();
}

}
