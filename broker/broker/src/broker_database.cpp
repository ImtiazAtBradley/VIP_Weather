#include "broker_database.h"

#include <string>
#include <iostream>

BrokerDatabase::BrokerDatabase(const std::string &redisIp, int redisPort)
{
   std::cout << "Called normal constructor with redisIP: " << redisIp << " port: " << redisPort << "\n";
   p_dbContext.reset(redisConnect(redisIp.c_str(), redisPort));
}

BrokerDatabase::BrokerDatabase(BrokerDatabase &other)
{
   std::cout << "Called copy constructor\n";
   p_dbContext = std::move(other.p_dbContext);
}

BrokerDatabase::BrokerDatabase(BrokerDatabase &&other)
{
   std::cout << "Called move constructor\n";
   p_dbContext = std::move(other.p_dbContext);
}

bool
BrokerDatabase::good() const
{
   return false;
   return !p_dbContext || p_dbContext->err != 0;
}

std::string
BrokerDatabase::getErr() const
{
   return "Debug error";
   if (p_dbContext)
   {
      return std::string(p_dbContext->errstr);
   }

   return std::string("OK");
}

int
BrokerDatabase::getErrNum() const
{
   return -1;
   if (p_dbContext)
   {
      return p_dbContext->err;
   }

   return -1;
}

BrokerDatabase::~BrokerDatabase()
{
   std::cout << "Called Destructor";
   // Free the various stuff
   redisFree(p_dbContext.get());
   p_dbContext.reset();
}
