#include "lazygl/widget/treeview/treeview_item.h"

namespace lazygl
{
TreeviewItem::TreeviewItem()
  : TreeviewItem("(noname)")
{
}

TreeviewItem::TreeviewItem(const std::string& name)
  : name_(name)
{
}

TreeviewItem::~TreeviewItem() = default;
}
