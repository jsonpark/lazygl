#ifndef LAZYGL_WIDGET_SPLIT_WIDGET_H_
#define LAZYGL_WIDGET_SPLIT_WIDGET_H_

#include <array>

#include "lazygl/widget/widget.h"
#include "lazygl/renderer/renderer.h"

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
private:
  class SplitRef
  {
  public:
    SplitRef() = delete;

    SplitRef(double& split)
      : split_(split)
    {
    }

    ~SplitRef() = default;

    operator double ()
    {
      return split_;
    }

    auto& operator = (double split)
    {
      if (split < 0.)
        split_ = 0.;
      else if (split > 1.)
        split_ = 1.;
      else
        split_ = split;

      return *this;
    }

  private:
    double& split_;
  };

public:
  SplitWidget()
    : Widget()
  {
  }

  SplitWidget(double x, double y, double width, double height)
    : Widget(x, y, width, height)
  {
  }

  ~SplitWidget() override
  {
  }

  // Specialized
  void UpdateChildrenPosSize() override
  {
  }

  void AttachWidget(int idx, std::shared_ptr<Widget> widget)
  {
    if (0 <= idx && idx <= 1)
    {
      Disconnect(shared_from_this(), widgets_[idx]);
      widgets_[idx] = widget;
      Connect(shared_from_this(), widgets_[idx]);
    }
  }

  void AttachWidget(std::shared_ptr<Widget> widget1, std::shared_ptr<Widget> widget2)
  {
    Disconnect(shared_from_this(), widgets_[0]);
    Disconnect(shared_from_this(), widgets_[1]);
    widgets_[0] = widget1;
    widgets_[1] = widget2;
    Connect(shared_from_this(), widgets_[0]);
    Connect(shared_from_this(), widgets_[1]);
  }

  void DetachWidget(int idx)
  {
    if (0 <= idx && idx <= 1)
    {
      Disconnect(shared_from_this(), widgets_[idx]);
      widgets_[idx] = nullptr;
    }
  }

  void DetachWidget()
  {
    Disconnect(shared_from_this(), widgets_[0]);
    Disconnect(shared_from_this(), widgets_[1]);
    widgets_[0] = nullptr;
    widgets_[1] = nullptr;
  }

  std::shared_ptr<Widget> GetSplitWidget(int idx)
  {
    if (0 <= idx && idx <= 1)
      return widgets_[idx];

    return nullptr;
  }

  SplitRef Split()
  {
    return SplitRef(split_);
  }

  auto& Gap()
  {
    return gap_;
  }

  void MouseHoldSplit()
  {
    mouse_holding_split_ = true;
  }

  void MouseReleaseSplit()
  {
    mouse_holding_split_ = false;
  }

  bool IsMouseHoldingSplit() const
  {
    return mouse_holding_split_;
  }

private:
  std::array<std::shared_ptr<Widget>, 2> widgets_;

  double split_ = 0.5;
  double gap_ = 8.;

  bool mouse_holding_split_ = false;
};

using HorizontalSplit = SplitWidget<Split::HORIZONTAL>;
using VerticalSplit = SplitWidget<Split::VERTICAL>;

// UpdateChildrenPosSize
template<>
void VerticalSplit::UpdateChildrenPosSize()
{
  const auto& pos = GetPos();
  const auto& size = GetSize();

  double width0 = size(0) * split_ - gap_ / 2.;
  double width1 = size(0) * (1. - split_) - gap_ / 2.;

  if (widgets_[0] != nullptr)
  {
    widgets_[0]->SetPos(0., 0.);
    widgets_[0]->SetSize(width0, size(1));
  }

  if (widgets_[1] != nullptr)
  {
    widgets_[1]->SetPos(width0 + gap_, 0.);
    widgets_[1]->SetSize(width1, size(1));
  }
}

template<>
void HorizontalSplit::UpdateChildrenPosSize()
{
  const auto& pos = GetPos();
  const auto& size = GetSize();

  double height0 = size(1) * split_ - gap_ / 2.;
  double height1 = size(1) * (1. - split_) - gap_ / 2.;

  if (widgets_[0] != nullptr)
  {
    widgets_[0]->SetPos(0., 0.);
    widgets_[0]->SetSize(size(0), height0);
  }

  if (widgets_[1] != nullptr)
  {
    widgets_[1]->SetPos(0., height0 + gap_);
    widgets_[1]->SetSize(size(0), height1);
  }
}
}

#endif // LAZYGL_WIDGET_SPLIT_WIDGET_H_
