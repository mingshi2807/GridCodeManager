#ifndef _LIB_EMS_H_
#define _LIB_EMS_H_
#include "spdlog/spdlog.h"

#include <string_view>

void
PostEmsMessage(const std::string_view mode)
{
  spdlog::info("[EmsCommunication] : {}", mode);
}
#endif //_LIB_EMS_H_
