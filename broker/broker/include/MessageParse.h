#pragma once

#include <optional>
#include <string>
#include "MessageResponse.h"

class MessageParse
{
 public:
   MessageParse();
   static std::optional<MessageResponse> parseMessage(const std::string &radioMessage);

 private:
};

