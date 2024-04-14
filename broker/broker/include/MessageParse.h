#pragma once

#include "WeatherData.h"
#include <optional>
#include <string>

class MessageParse
{
 public:
   MessageParse();
   static std::optional<WeatherData> parseMessage(const std::string &radioMessage);

 private:
};
