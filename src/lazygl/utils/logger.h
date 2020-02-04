#ifndef LAZYGL_UTILS_LOGGER_H_
#define LAZYGL_UTILS_LOGGER_H_

#include <iostream>

namespace lazygl
{
class Logger
{
public:
  static void endl(Logger& logger);

public:
  Logger();

  explicit Logger(const std::string& name);

  ~Logger();

  template <typename T>
  Logger& operator << (T&& v)
  {
    if (first_input_)
    {
      std::cout << name_ << ": ";
      first_input_ = false;
    }

    std::cout << v;

    return *this;
  }

  Logger& operator << (decltype(std::endl<std::ostream::char_type, std::ostream::traits_type>) e)
  {
    std::cout << std::endl;

    return *this;
  }

  Logger& operator << (decltype(endl) e)
  {
    std::cout << std::endl;
    first_input_ = true;

    return *this;
  }

private:
  bool first_input_ = true;
  std::string name_;
};
}

#endif // LAZYGL_UTILS_LOGGER_H_
