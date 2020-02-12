#include "lazygl/widget/pushbutton_widget.h"

namespace lazygl
{
PushbuttonWidget::PushbuttonWidget()
  : PushbuttonWidget("")
{
}

PushbuttonWidget::PushbuttonWidget(const std::string& text)
  : Widget(), text_(text)
{
}

PushbuttonWidget::PushbuttonWidget(double x, double y, double width, double height)
  : PushbuttonWidget(x, y, width, height, "")
{
}

PushbuttonWidget::PushbuttonWidget(double x, double y, double width, double height, const std::string& text)
  : Widget(x, y, width, height), text_(text)
{
}

PushbuttonWidget::~PushbuttonWidget() = default;
}
