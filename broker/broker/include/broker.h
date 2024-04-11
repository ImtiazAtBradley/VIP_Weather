#pragma once

#include "broker_database.h"

#include <chrono>
#include <string>

class Broker
{
 public:
   Broker(BrokerDatabase &brokerDb, std::chrono::milliseconds schedulerTimeMs);
   Broker(BrokerDatabase &&brokerDb);
   void printProgramHeader();
   void help();
   bool validateUserInput(int argc, char *argv[]);
   std::string getDbError();
   bool dbGood();
   bool runScheduler();

 private:
   bool fileExists(const std::string &name);
   void runTasks();

   BrokerDatabase m_database;
   std::chrono::milliseconds m_frequency_ms;
   std::chrono::time_point<std::chrono::high_resolution_clock> m_lastRunTime_ms;
};
