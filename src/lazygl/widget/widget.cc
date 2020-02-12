#include "lazygl/widget/widget.h"

namespace lazygl
{
void Widget::Connect(std::shared_ptr<Widget> parent, std::shared_ptr<Widget> child)
{
  if (child == nullptr)
    return;

  auto old_parent = child->parent_.lock();
  if (old_parent)
    old_parent->DeleteChild(child);

  if (parent != nullptr)
    parent->AddChild(child);
  child->SetParent(parent);
}

void Widget::Disconnect(std::shared_ptr<Widget> parent, std::shared_ptr<Widget> child)
{
  if (child == nullptr || parent == nullptr)
    return;

  parent->DeleteChild(child);
  child->SetParent(nullptr);
}


Widget::Widget()
{
}

Widget::Widget(double width, double height)
  : size_(width, height)
{
}

Widget::Widget(double x, double y, double width, double height)
  : pos_(x, y), size_(width, height)
{
}

Widget::~Widget() = default;

void Widget::SetParent(std::shared_ptr<Widget> parent)
{
  parent_ = parent;
}

void Widget::AddChild(std::shared_ptr<Widget> child)
{
  children_.push_back(child);
}

void Widget::DeleteChild(std::shared_ptr<Widget> child)
{
  // TODO: using hash table to delete child in O(1)
  for (size_t i = 0; i < children_.size(); i++)
  {
    if (children_[i] == child)
    {
      for (size_t j = i; j < children_.size() - 1; j++)
        children_[j] = children_[j + 1];
      children_.pop_back();
      break;
    }
  }
}

void Widget::UpdatePosSize()
{
  UpdateChildrenPosSize();

  for (auto child : children_)
  {
    child->root_pos_ = root_pos_ + child->GetPos();
    child->UpdatePosSize();
  }
}

void Widget::UpdateChildrenPosSize()
{
}
}
