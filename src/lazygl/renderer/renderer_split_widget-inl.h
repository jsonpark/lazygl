#include "lazygl/renderer/renderer.h"

namespace lazygl
{
// MouseButton
template <>
void Renderer::WidgetMouseButton(std::shared_ptr<VerticalSplit> widget, input::MouseButton button, input::MouseAction action, double x, double y)
{
  if (action == input::MouseAction::PRESS)
  {
    const auto& size = widget->GetSize();

    double width0 = size(0) * widget->Split() - widget->Gap() / 2.;

    if (width0 <= x && x <= width0 + widget->Gap())
    {
      if (button == input::MouseButton::LEFT)
      {
        mouse_active_widget_ = widget;
        mouse_active_pos_ = mouse_pos_;
        widget->MouseHoldSplit();
      }
    }

    else if (x < width0)
      WidgetMouseButton(widget->GetSplitWidget(0), button, action, x, y);

    else if (x > width0 + widget->Gap())
      WidgetMouseButton(widget->GetSplitWidget(1), button, action, x - width0 - widget->Gap(), y);
  }
  else
  {
    mouse_active_widget_ = nullptr;
    widget->MouseReleaseSplit();
  }
}

template <>
void Renderer::WidgetMouseButton(std::shared_ptr<HorizontalSplit> widget, input::MouseButton button, input::MouseAction action, double x, double y)
{
  if (action == input::MouseAction::PRESS)
  {
    const auto& size = widget->GetSize();

    double height0 = size(1) * widget->Split() - widget->Gap() / 2.;

    if (height0 <= y && y <= height0 + widget->Gap())
    {
      if (button == input::MouseButton::LEFT)
      {
        mouse_active_widget_ = widget;
        widget->MouseHoldSplit();
      }
    }

    else if (y < height0)
      WidgetMouseButton(widget->GetSplitWidget(0), button, action, x, y);

    else if (y > height0 + widget->Gap())
      WidgetMouseButton(widget->GetSplitWidget(1), button, action, x, y - height0 - widget->Gap());
  }
  else
  {
    mouse_active_widget_ = nullptr;
    widget->MouseReleaseSplit();
  }
}


// MouseMove
template <>
void Renderer::WidgetMouseMove(std::shared_ptr<VerticalSplit> widget, double x, double y)
{
  const auto& size = widget->GetSize();

  if (widget->IsMouseHoldingSplit())
  {
    widget->Split() = x / size(0);
  }
}

template <>
void Renderer::WidgetMouseMove(std::shared_ptr<HorizontalSplit> widget, double x, double y)
{
  const auto& size = widget->GetSize();

  if (widget->IsMouseHoldingSplit())
  {
    widget->Split() = y / size(1);
  }
}


// Draw
template <>
void Renderer::WidgetDraw(std::shared_ptr<VerticalSplit> widget)
{
  const auto& size = widget->GetSize();
  const auto& root_pos = widget->GetRootPos();

  double width0 = size(0) * widget->Split() - widget->Gap() / 2.;

  DrawRect(root_pos(0) + width0, Height() - root_pos(1) - size(1), widget->Gap(), size(1), Vector3f(0.8f, 0.8f, 0.8f));
}

template <>
void Renderer::WidgetDraw(std::shared_ptr<HorizontalSplit> widget)
{
  const auto& size = widget->GetSize();
  const auto& root_pos = widget->GetRootPos();

  double height0 = size(1) * widget->Split() - widget->Gap() / 2.;

  DrawRect(root_pos(0), Height() - root_pos(1) - height0 - widget->Gap(), size(0), widget->Gap(), Vector3f(0.8f, 0.8f, 0.8f));
}
}
