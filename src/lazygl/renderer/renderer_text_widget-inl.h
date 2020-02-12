#include "lazygl/renderer/renderer.h"

namespace lazygl
{
template <>
void Renderer::WidgetDraw(std::shared_ptr<TextWidget> widget)
{
  const auto& size = widget->GetSize();
  const auto& root_pos = widget->GetRootPos();

  RenderText(root_pos(0), Height() - root_pos(1), widget->FontSize(), widget->GetText(), widget->GetColor());
}
}
