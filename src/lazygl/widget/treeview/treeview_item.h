#ifndef LAZYGL_WIDGET_TREEVIEW_TREEVIEW_ITEM_H_
#define LAZYGL_WIDGET_TREEVIEW_TREEVIEW_ITEM_H_

#include <string>

namespace lazygl
{
class TreeviewItem
{
public:
  TreeviewItem();

  TreeviewItem(const std::string& name);

  ~TreeviewItem();

private:
  std::string name_;
};
}

#endif // LAZYGL_WIDGET_TREEVIEW_TREEVIEW_ITEM_H_
