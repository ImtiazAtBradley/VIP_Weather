#pragma once

#include <spdlog/spdlog.h>

#include <filesystem>
#include <fstream>

enum class WSLogLevel {
  DEBUG,
  INFO,
  ERROR,
};

class WSLogger
{
 private:
   /* data */
   void writeLog(std::string msg, WSLogLevel lvl);
   std::vector<spdlog::sink_ptr> m_sinks;
   std::shared_ptr<spdlog::logger> m_sbdlog;
 public:
  WSLogger(std::filesystem::path logPath, int maxFileSize, int maxFiles);
   void Debug(std::string msg);
   void Info(std::string msg);
   void Error(std::string msg);
};