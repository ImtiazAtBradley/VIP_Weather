#pragma once

namespace bc_broker
{
   namespace redis
   {
      constexpr static char ip[] = "127.0.0.1";
      constexpr static int port = 6379;
   } // namespace redis

   namespace version
   {
      constexpr static int major = 0;
      constexpr static int minor = 1;
      constexpr static int patch = 0;
   } // namespace version
} // namespace bc_broker
