#pragma once

#include "MessageResponse.h"
#include "wslogger.h"

#include <chrono>
#include <string>
#include <filesystem>

class Broker
{
 public:
   // STATIC MEMBERS
   static void printProgramHeader();
   static void help();

   Broker(std::chrono::milliseconds schedulerTimeMs,
          std::string path,
          std::string url,
          std::string key,
          std::filesystem::path logFilePath,
          int logFileSize,
          int numLogFiles);
   ~Broker();
   bool serialFileGood();
   bool runScheduler();
   bool setupSerialPort();
   bool writeToPort(const std::string &val);
   std::string readFromPort();

   WSLogger m_wsLog;

 private:
   void runTasks();
   bool postToAPI(WeatherData data, std::string url, std::string apiKey);

   int m_fd = -1;
   bool m_serialUp = false;

   std::string m_url;
   std::string m_apiKey;
   std::filesystem::path m_filePath;
   std::chrono::milliseconds m_frequency_ms;
   std::chrono::time_point<std::chrono::high_resolution_clock> m_lastRunTime_ms;
};
