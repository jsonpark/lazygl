#ifndef LAZYGL_WIDGET_TREEVIEW_WIDGET_H_
#define LAZYGL_WIDGET_TREEVIEW_WIDGET_H_

#include <array>

#include "lazygl/widget/widget.h"
#include "lazygl/renderer/renderer.h"

namespace lazygl
{
class TreeviewWidget : public Widget
{
public:
  TreeviewWidget();

  ~TreeviewWidget() override;

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

#endif // LAZYGL_WIDGET_TREEVIEW_WIDGET_H_
