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
   std::shared_ptr<spdlog::logger> m_spdlog;
 public:
  WSLogger(std::filesystem::path logPath, int maxFileSize, int maxFiles);
   void Debug(std::string msg);
   void Info(std::string msg);
   void Error(std::string msg);
};