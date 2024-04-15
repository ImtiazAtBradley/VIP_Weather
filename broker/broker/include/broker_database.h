#pragma once

#include <string>
#include <hiredis/hiredis.h>
#include <memory>
#include "WeatherData.h"

// "functor"
// This makes a structure which is default constructuble. A type that acts like a function
struct RedisDbDeleter
{
   void
   operator()(redisContext *p)
   {
      redisFree(p);
   }
};

class BrokerDatabase
{
 public:
   BrokerDatabase(const std::string &redisIp, const int redisPort, const int timeout_s);
   BrokerDatabase(BrokerDatabase &other);
   BrokerDatabase(BrokerDatabase &&other);
   bool good() const;
   std::string getErr() const;
   int getErrNum() const;
   bool postStreamData(const int id, const WeatherData &data);

 private:
   static void freeRedis(redisContext *ptr);

   std::unique_ptr<redisContext, RedisDbDeleter> p_dbContext;
};
