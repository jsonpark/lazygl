#ifndef LAZYGL_WIDGET_LAYOUT_H_
#define LAZYGL_WIDGET_LAYOUT_H_

#include "lazygl/widget/widget.h"

namespace lazygl
{
enum class Split
{
  HORIZONTAL,
  VERTICAL
};

template <Split split>
class SplitWidget : public Widget
{
public:
  SplitWidget()
    : Widget()
  {
  }

  SplitWidget(double x, double y, double width, double height)
    : Widget(x, y, width, height)
  {
  }

  SplitWidget(std::shared_ptr<Widget> widget1, std::shared_ptr<Widget> widget2)
    : Widget(),
    widgets_({ widget1, widget2 }),
    need_connect_widgets_(true)
  {
  }

  SplitWidget(double x, double y, double width, double height, std::shared_ptr<Widget> widget1, std::shared_ptr<Widget> widget2)
    : Widget(x, y, width, height),
    widgets_({ widget1, widget2 }),
    need_connect_widgets_(true)
  {
  }

  ~SplitWidget() override
  {
  }

  // Input functions
  void MouseButton(input::MouseButton button, input::MouseAction action, double x, double y) override;
  void MouseMove(double x, double y) override;

  // Specialized
  void UpdateChildrenPosSize() override
  {
  }

  void AttachWidget(int idx, std::shared_ptr<Widget> widget)
  {
    if (need_connect_widgets_)
      ConnectWidgets();

    if (0 <= idx && idx <= 1)
    {
      Disconnect(shared_from_this(), widgets_[1]);
      widgets_[idx] = widget;
      need_connect_widgets_ = true;
    }
  }

  void AttachWidget(std::shared_ptr<Widget> widget1, std::shared_ptr<Widget> widget2)
  {
    if (need_connect_widgets_)
      ConnectWidgets();

    Disconnect(shared_from_this(), widgets_[0]);
    Disconnect(shared_from_this(), widgets_[1]);
    widgets_[0] = widget1;
    widgets_[1] = widget2;
    need_connect_widgets_ = true;
  }

  void DetachWidget(int idx)
  {
    if (need_connect_widgets_)
      ConnectWidgets();

    if (0 <= idx && idx <= 1)
    {
      Disconnect(shared_from_this(), widgets_[idx]);
      widgets_[idx] = nullptr;
    }
  }

  void DetachWidget()
  {
    if (need_connect_widgets_)
      ConnectWidgets();

    Disconnect(shared_from_this(), widgets_[0]);
    Disconnect(shared_from_this(), widgets_[1]);
    widgets_[0] = nullptr;
    widgets_[1] = nullptr;
  }

  void ConnectWidgets()
  {
    if (need_connect_widgets_)
    {
      Connect(shared_from_this(), widgets_[0]);
      Connect(shared_from_this(), widgets_[1]);
      need_connect_widgets_ = false;
    }
  }

private:
  std::array<std::shared_ptr<Widget>, 2> widgets_;
  bool need_connect_widgets_ = false;

  double split_ = 0.5;
  double gap_ = 10;

  bool mouse_holding_split_ = false;
};

using HorizontalSplit = SplitWidget<Split::HORIZONTAL>;
using VerticalSplit = SplitWidget<Split::VERTICAL>;

// UpdateChildrenPosSize
template<>
void HorizontalSplit::UpdateChildrenPosSize()
{
  const auto& pos = GetPos();
  const auto& size = GetSize();

  double width0 = size(0) * split_ - gap_ / 2.;
  double width1 = size(0) * (1. - split_) - gap_ / 2.;

  widgets_[0]->SetPos(pos(0), pos(1));
  widgets_[0]->SetSize(width0, size(1));
  widgets_[1]->SetPos(pos(0) + width0 + gap_, pos(1));
  widgets_[1]->SetSize(width1, size(1));
}

template<>
void VerticalSplit::UpdateChildrenPosSize()
{
  const auto& pos = GetPos();
  const auto& size = GetSize();

  double height0 = size(1) * split_ - gap_ / 2.;
  double height1 = size(1) * (1. - split_) - gap_ / 2.;

  widgets_[0]->SetPos(0., height1 + gap_);
  widgets_[0]->SetSize(size(0), height0);
  widgets_[1]->SetPos(0., 0.);
  widgets_[1]->SetSize(size(0), height1);
}


// MouseButton
template<>
void HorizontalSplit::MouseButton(input::MouseButton button, input::MouseAction action, double x, double y)
{
  if (button == input::MouseButton::LEFT)
  {
    if (action == input::MouseAction::PRESS)
    {
      const auto& size = GetSize();

      double width0 = size(0) * split_ - gap_ / 2.;

      if (width0 <= x && x <= width0 + gap_)
        mouse_holding_split_ = true;
    }
    else
      mouse_holding_split_ = false;
  }
}

template<>
void VerticalSplit::MouseButton(input::MouseButton button, input::MouseAction action, double x, double y)
{
  if (button == input::MouseButton::LEFT)
  {
    if (action == input::MouseAction::PRESS)
    {
      const auto& size = GetSize();

      double height0 = size(1) * split_ - gap_ / 2.;

      if (height0 <= y && y <= height0 + gap_)
        mouse_holding_split_ = true;
    }
    else
      mouse_holding_split_ = false;
  }
}


// MouseMove
template<>
void HorizontalSplit::MouseMove(double x, double y)
{
  const auto& size = GetSize();

  if (mouse_holding_split_)
  {
    split_ = x / size(0);
  }
}

template<>
void VerticalSplit::MouseMove(double x, double y)
{
  const auto& size = GetSize();

  if (mouse_holding_split_)
  {
    split_ = 1. - y / size(1);
  }
}
}

#endif // LAZYGL_WIDGET_LAYOUT_H_
