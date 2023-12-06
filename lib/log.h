#ifndef _LIB_LOG_H_
#define _LIB_LOG_H_
#include "spdlog/spdlog.h"

void
Log_display(const std::string_view message)
{
  // todos add fake applicative command
  spdlog::info(std::string("[Log]: ") + std::string(message.data()));
}
#endif //_LIB_LOG_H_
