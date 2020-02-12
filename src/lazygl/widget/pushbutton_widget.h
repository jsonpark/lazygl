#ifndef LAZYGL_WIDGET_PUSHBUTTON_WIDGET_H_
#define LAZYGL_WIDGET_PUSHBUTTON_WIDGET_H_

#include <array>

#include "lazygl/widget/widget.h"
#include "lazygl/renderer/renderer.h"

namespace lazygl
{
class PushbuttonWidget : public Widget
{
public:
  PushbuttonWidget();

  PushbuttonWidget(const std::string& text);

  PushbuttonWidget(double x, double y, double width, double height);

  PushbuttonWidget(double x, double y, double width, double height, const std::string& text);

  ~PushbuttonWidget() override;

  const auto& GetText() const
  {
    return text_;
  }

  void SetText(const std::string& text)
  {
    text_ = text;
  }

private:
  std::string text_;
};
}

#endif // LAZYGL_WIDGET_PUSHBUTTON_WIDGET_H_
