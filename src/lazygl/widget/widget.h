#ifndef LAZYGL_WIDGET_WIDGET_H_
#define LAZYGL_WIDGET_WIDGET_H_

#include <vector>
#include <memory>

#include "lazygl/types.h"
#include "lazygl/window/input/inputs.h"

namespace lazygl
{
class Widget : public std::enable_shared_from_this<Widget>
{
public:
  static void Connect(std::shared_ptr<Widget> parent, std::shared_ptr<Widget> child);
  static void Disonnect(std::shared_ptr<Widget> parent, std::shared_ptr<Widget> child);

public:
  Widget();

  Widget(double width, double height);

  Widget(double x, double y, double width, double height);

  virtual ~Widget();

  void SetParent(std::shared_ptr<Widget> parent);
  void AddChild(std::shared_ptr<Widget> child);
  void DeleteChild(std::shared_ptr<Widget> child);

  const auto& GetPos() const
  {
    return pos_;
  }

  const auto& GetSize() const
  {
    return size_;
  }

  void SetPos(double x, double y)
  {
    pos_(0) = x;
    pos_(1) = y;
  }

  void SetPos(const Vector2d& pos)
  {
    pos_ = pos;
  }

  void SetSize(double width, double height)
  {
    size_(0) = width;
    size_(1) = height;
  }

  void SetSize(const Vector2d& size)
  {
    size_ = size;
  }

  void Show()
  {
    shown_ = true;
  }

  void Hide()
  {
    shown_ = false;
  }

  void ToggleShow()
  {
    shown_ ^= true;
  }

  void Update();
  
  // Input functions
  virtual void MouseButton(input::MouseButton button, input::MouseAction action, double x, double y);
  virtual void MouseMove(double x, double y);
  virtual void Keyboard(input::Key key, input::KeyAction action);

  // Resize update function
  virtual void UpdateChildrenPosSize();

protected:
  auto GetChild(int idx) const
  {
    return children_[idx];
  }

private:
  Vector2d pos_{ 0., 0. };
  Vector2d size_{ 0., 0. };

  std::weak_ptr<Widget> parent_;
  std::vector<std::shared_ptr<Widget>> children_;

  bool shown_ = true;
};
}

#endif // LAZYGL_WIDGET_WIDGET_H_
