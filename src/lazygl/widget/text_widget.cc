#include "lazygl/widget/text_widget.h"

namespace lazygl
{
TextWidget::TextWidget()
  : Widget()
{
}

TextWidget::TextWidget(double x, double y)
  : TextWidget(x, y, "")
{
}

TextWidget::TextWidget(double x, double y, const std::string& text)
  : Widget(x, y, 0, 0), text_(text)
{
}

TextWidget::TextWidget(double x, double y, double width, double height)
  : TextWidget(x, y, width, height, "")
{
}

TextWidget::TextWidget(double x, double y, double width, double height, const std::string& text)
  : Widget(x, y, width, height), text_(text)
{
}

TextWidget::~TextWidget() = default;
}
