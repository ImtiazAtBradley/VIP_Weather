#pragma once

#include "broker_database.h"

#include <chrono>
#include <string>
#include <filesystem>

class Broker
{
 public:
   Broker(BrokerDatabase &brokerDb, std::chrono::milliseconds schedulerTimeMs, std::string path);
   Broker(BrokerDatabase &&brokerDb);
   ~Broker();
   static void printProgramHeader();
   static void help();
   std::string getDbError();
   bool dbGood();
   bool serialFileGood();
   bool runScheduler();
   bool setupSerialPort();
   bool writeToPort(const std::string &val);

 private:
   bool fileExists();
   void runTasks();

   int m_fd = -1;
   bool m_serialUp = false;
   BrokerDatabase m_database;
   std::filesystem::path m_filePath;
   std::chrono::milliseconds m_frequency_ms;
   std::chrono::time_point<std::chrono::high_resolution_clock> m_lastRunTime_ms;
};
