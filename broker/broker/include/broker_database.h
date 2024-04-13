#pragma once

#include <string>
#include <hiredis/hiredis.h>
#include <memory>

class BrokerDatabase
{
 public:
   BrokerDatabase(const std::string &redisIp, int redisPort);
   BrokerDatabase(BrokerDatabase &other);
   BrokerDatabase(BrokerDatabase &&other);
   bool good() const;
   std::string getErr() const;
   int getErrNum() const;
   ~BrokerDatabase();

 private:
   std::unique_ptr<redisContext> p_dbContext;
};

