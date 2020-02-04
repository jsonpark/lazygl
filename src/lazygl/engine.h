#ifndef LAZYGL_ENGINE_H_
#define LAZYGL_ENGINE_H_

#include <memory>
#include <vector>

namespace lazygl
{
class Window;

class Engine
{
public:
  Engine();
  ~Engine();

  template <class WindowDerived, typename ... Ts>
  std::shared_ptr<WindowDerived> CreateWindowPtr(Ts&&... v)
  {
    auto window = std::make_shared<WindowDerived>(v...);

    windows_.push_back(window);

    return window;
  }

  void Run();

private:
  bool ShouldClose();

  std::vector<std::shared_ptr<Window>> windows_;
};
}

#endif // LAZYGL_ENGINE_H_
