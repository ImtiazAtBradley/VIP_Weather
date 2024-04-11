#include "broker.h"
#include "broker_database.h"
#include "constants.h"
#include <chrono>
#include <ratio>
#include <string>
#include <iostream>
#include <sys/stat.h>

Broker::Broker(BrokerDatabase &brokerDb, std::chrono::milliseconds schedulerTimeMs)
    : m_database(brokerDb), m_frequency_ms(schedulerTimeMs)
{
}

Broker::Broker(BrokerDatabase &&brokerDb) : m_database(brokerDb) {}

void
Broker::help()
{
   std::cout << "Bradley Cast Broker - Access point of weather stations to "
                "server database\n"
             << "usage: bradley-cast-broker [serial dev file]\n"
             << "serial dev file: A device file representing a serial adapter, "
                "like '/dev/ttyx'\n";
}

bool
Broker::validateUserInput(int argc, char *argv[])
{
   if (argc != 2)
   {
      return false;
   }

   /**
   * CHECK IF FILENAME EXISTS
   *
   * Note that this function (access) is NOT safe to use typically, because in
   * the time that the file is momentarily open, a user can execute an attack in
   * that short time to manipulate it. For our purposes, we really don't care.
   */
   if (!fileExists(argv[1]))
   {
      return false;
   }

   return true;
}

void
Broker::printProgramHeader()
{
   std::cout << "Bradley Cast Broker V" << bc_broker::version::major << "." << bc_broker::version::minor << "."
             << bc_broker::version::patch << "\n";
}

bool
Broker::dbGood()
{
   return m_database.good();
}

std::string
Broker::getDbError()
{
   return m_database.getErr();
}

bool
Broker::runScheduler()
{

   auto now = std::chrono::high_resolution_clock::now();

   if (std::chrono::duration_cast<std::chrono::milliseconds>(now - m_lastRunTime_ms) > m_frequency_ms)
   {
      // Run internal tasks
      runTasks();
      m_lastRunTime_ms = std::chrono::high_resolution_clock::now();
      return true; // Ran tasks
   }

   return false;
}

// ============================== PRIVATE FUNCTIONS ==============================

/*
 * Source:
 * https://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exists-using-standard-c-c11-14-17-c
 */
bool
Broker::fileExists(const std::string &name)
{
   struct stat buffer;
   // NOTE: This *does* check for existence, but has quite a few other
   //   failure modes. For now, we'll just check for existence with this
   return (stat(name.c_str(), &buffer) == 0);
}

void
Broker::runTasks()
{
}
