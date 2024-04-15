#include "broker_database.h"

#include <hiredis/hiredis.h>
#include <sys/time.h>
#include <string>
#include <iostream>

BrokerDatabase::BrokerDatabase(const std::string &redisIp, const int redisPort, const int timeout_s)
{
   std::cout << "[DEBUG] Called normal constructor with redisIP: " << redisIp << " port: " << redisPort << "\n";

   struct timeval timeout = {};
   timeout.tv_sec = timeout_s;
   redisContext *redisp = redisConnectWithTimeout(redisIp.c_str(), redisPort, timeout);

   if (redisp != nullptr)
   {
      p_dbContext.reset(redisp);
   }
}

BrokerDatabase::BrokerDatabase(BrokerDatabase &other)
{
   std::cout << "[DEBUG] Called copy constructor\n";
   p_dbContext = std::move(other.p_dbContext);
}

BrokerDatabase::BrokerDatabase(BrokerDatabase &&other)
{
   std::cout << "[DEBUG] Called move constructor\n";
   p_dbContext = std::move(other.p_dbContext);
}

bool
BrokerDatabase::good() const
{
   return !p_dbContext || p_dbContext->err == 0;
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

void
BrokerDatabase::freeRedis(redisContext *ptr)
{
   std::cout << "[DEBUG] Called redis deleter\n";
   redisFree(ptr);
}

bool
BrokerDatabase::postStreamData(const int id, const WeatherData &data)
{
   void *pReply;

   std::string lightLevel = [data]()
   {
      switch (data.m_lightLevel)
      {
         case SUNNY:
            return "SUNNY";
            break;
         case DARK:
            return "DARK";
            break;
      }

      return "SUNNY";
   }();

   pReply = redisCommand(
       p_dbContext.get(),
       // This will save 15 days of data
       "XADD weather-station:%d MAXLEN ~ 720 * temp_c %.02f humid_prcnt %.02f pressure_kpa %.02f is_raining %d light_level %s",
       id,
       data.m_temp_c,
       data.m_humid,
       data.m_pressure_kpa,
       data.m_isRaining,
       lightLevel.c_str());

   if (pReply == NULL){
      return false;
   }

   freeReplyObject(pReply);
   return true;
}
