#include "wslogger.h"

#include "utils.h"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_sinks.h>

#include <sstream>

WSLogger::WSLogger(std::filesystem::path logPath, int maxFileSize, int maxFiles)
{
   std::vector<spdlog::sink_ptr> sinks;

   sinks.push_back(std::make_shared<spdlog::sinks::rotating_file_sink_st>(logPath.string(), maxFileSize, maxFiles));
   sinks.push_back(std::make_shared<spdlog::sinks::stdout_sink_st>());


   m_spdlog = std::make_shared<spdlog::logger>("ws-logger", begin(sinks), end(sinks));
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
      m_spdlog->debug("Testing");
      break;
   case WSLogLevel::INFO:
      m_spdlog->info("Testing");
      break;
   case WSLogLevel::ERROR:
      m_spdlog->error("Testing");
      break;
   }

}