#ifndef LAZYGL_WIDGET_TEXT_WIDGET_H_
#define LAZYGL_WIDGET_TEXT_WIDGET_H_

#include <array>

#include "lazygl/widget/widget.h"
#include "lazygl/renderer/renderer.h"

namespace lazygl
{
class TextWidget : public Widget
{
public:
  TextWidget();

  TextWidget(double x, double y);

  TextWidget(double x, double y, const std::string& text);

  TextWidget(double x, double y, double width, double height);

  TextWidget(double x, double y, double width, double height, const std::string& text);

  ~TextWidget() override;

  auto& FontSize()
  {
    return size_;
  }

  const auto& GetText() const
  {
    return text_;
  }

  const auto& GetColor() const
  {
    return color_;
  }

private:
  std::string text_;
  double size_ = 10.;
  Vector3f color_{ 0.f, 0.f, 0.f };
};
}

#endif // LAZYGL_WIDGET_TEXT_WIDGET_H_
