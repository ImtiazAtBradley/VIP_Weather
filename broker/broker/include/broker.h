#pragma once

#include "MessageResponse.h"

#include <chrono>
#include <string>
#include <filesystem>

class Broker
{
 public:
   // STATIC MEMBERS
   static bool PostToAPI(WeatherData data, std::string url, std::string apiKey);
   static void printProgramHeader();
   static void help();

   Broker(std::chrono::milliseconds schedulerTimeMs, std::string path, std::string url, std::string key);
   ~Broker();
   bool serialFileGood();
   bool runScheduler();
   bool setupSerialPort();
   bool writeToPort(const std::string &val);
   std::string readFromPort();

 private:
   void runTasks();

   int m_fd = -1;
   bool m_serialUp = false;
   std::string m_url;
   std::string m_apiKey;
   std::filesystem::path m_filePath;
   std::chrono::milliseconds m_frequency_ms;
   std::chrono::time_point<std::chrono::high_resolution_clock> m_lastRunTime_ms;
};
