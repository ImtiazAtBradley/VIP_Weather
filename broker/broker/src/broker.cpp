#include "broker.h"
#include "broker_database.h"
#include "constants.h"
#include <chrono>
#include <ratio>
#include <stdexcept>
#include <string>
#include <iostream>
#include <sys/stat.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>

Broker::Broker(BrokerDatabase &brokerDb, std::chrono::milliseconds schedulerTimeMs, std::string path)
    : m_database(brokerDb), m_frequency_ms(schedulerTimeMs), m_filePath(path)
{
}

Broker::Broker(BrokerDatabase &&brokerDb) : m_database(brokerDb) {}

void
Broker::help()
{
   std::cout << "BRADLEY CAST BROKER - ACCESS POINT OF WEATHER STATIONS TO "
                "SERVER DATABASE\n\n"
             << "AUTHORS:\n\t BRADLEY UNIVERSITY ECE398 WEATHER STATION PROJECT GROUP 2024\n"
             << "USAGE: \n\tbradley-cast-broker [serial dev file]\n\n"
             << "serial dev file: A device file representing a serial adapter, "
                "like '/dev/ttyx'\n";
}

void
Broker::printProgramHeader()
{
   std::cout << "BRADLEY CAST BROKER V" << bc_broker::version::major << "." << bc_broker::version::minor << "."
             << bc_broker::version::patch << "\n";
}

bool
Broker::dbGood()
{
   return m_database.good();
}

bool Broker::serialFileGood(){
   return fileExists();
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

bool Broker::setupSerialPort(){
   int file, rc;
   struct termios options;

   file = open(m_filePath.c_str(), O_RDWR);
   if (file < 0){
      return false;
   }
   m_fd = file;

   // Set baud rate to 115200
   cfsetispeed(&options, B115200);
   cfsetospeed(&options, B115200);

   // Input settings
   options.c_iflag &= ~(IGNPAR | IGNBRK | BRKINT | ISTRIP | INLCR | IXON | INPCK);
   options.c_iflag |= (IGNCR);
   options.c_oflag &= ~(OPOST | ONLCR | OCRNL | ONLRET);
   options.c_oflag |= (ONOCR);
   options.c_cflag &= (CSIZE | CSTOPB | PARENB);
   options.c_cflag |= (CS8);
   options.c_lflag &= ~(ISIG | ECHO | ECHOE | ECHOK | ECHONL | IEXTEN);
   options.c_lflag |= (ICANON);

   rc = tcsetattr(file, TCSANOW, &options);

   if (rc == 0){
      m_serialUp = true;
      return true;
   } else {
      return false;
   }
}

bool Broker::writeToPort(const std::string& val){
   if (!m_serialUp){
      if(!setupSerialPort()){
         return false;
      }
   }

   if (val.size() <= 0){
      return false;
   }

   size_t txNum = write(m_fd, val.c_str(), val.size());

   return txNum == val.size();
}

// ============================== PRIVATE FUNCTIONS ==============================

/*
 * Source:
 * https://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exists-using-standard-c-c11-14-17-c
 */
bool
Broker::fileExists()
{
   struct stat buffer;
   return (stat(m_filePath.c_str(), &buffer) == 0);
}

void Broker::runTasks() {
   // Do nothing for now
}

Broker::~Broker() {
   // Close file
   close(m_fd);
}
