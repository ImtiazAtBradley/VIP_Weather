#include "wslogger.h"

#include "utils.h"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <sstream>

WSLogger::WSLogger(std::filesystem::path logPath, int maxFileSize, int maxFiles)
{
   m_sinks.push_back(std::make_shared<spdlog::sinks::rotating_file_sink_st>("ws-broker-file", logPath, maxFileSize, maxFiles));
   m_sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_st>(spdlog::stdout_color_st("ws-broker-console")));

   m_sbdlog = std::make_shared<spdlog::logger>("ws-logger", begin(m_sinks), end(m_sinks));
}

void
WSLogger::Debug(std::string msg)
{
   writeLog(msg, WSLogLevel::DEBUG);
}

void
WSLogger::Info(std::string msg)
{
   writeLog(msg, WSLogLevel::INFO);
}

void
WSLogger::Error(std::string msg)
{
   writeLog(msg, WSLogLevel::ERROR);
}

// =============== PRIVATE ===============

void
WSLogger::writeLog(std::string msg, WSLogLevel lvl)
{
   std::stringstream logMsg{};
   logMsg << "[" << GetIso8601Time() << "] " << msg;

   switch (lvl)
   {
   case WSLogLevel::DEBUG:
      m_sbdlog->debug(logMsg);
      break;
   case WSLogLevel::INFO:
      m_sbdlog->info(logMsg);
      break;
   case WSLogLevel::ERROR:
      m_sbdlog->error(logMsg);
      break;
   }

}