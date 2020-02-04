#include "lazygl/utils/logger.h"

namespace lazygl
{
void Logger::endl(Logger& logger)
{
}

Logger::Logger() = default;

Logger::Logger(const std::string& name)
  : name_(name)
{
}

Logger::~Logger()
{
}
}
