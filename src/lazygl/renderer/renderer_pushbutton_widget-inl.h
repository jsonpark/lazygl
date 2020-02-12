#include "lazygl/renderer/renderer.h"

namespace lazygl
{
template <>
void Renderer::WidgetMouseButton(std::shared_ptr<PushbuttonWidget> widget, input::MouseButton button, input::MouseAction action, double x, double y)
{
}

template <>
void Renderer::WidgetMouseMove(std::shared_ptr<PushbuttonWidget> widget, double x, double y)
{
}

template <>
void Renderer::WidgetDraw(std::shared_ptr<PushbuttonWidget> widget)
{
  const auto& size = widget->GetSize();
  const auto& root_pos = widget->GetRootPos();

  // TODO: middle align
  constexpr double middle_align_diff = 10. * 4. / 3. / 2.;

  RenderText(root_pos(0), Height() - root_pos(1) - size(1) / 2. - middle_align_diff, 10., widget->GetText());
}
}
