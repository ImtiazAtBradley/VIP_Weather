#include "broker.h"
#include "MessageParse.h"
#include "MessageResponse.h"
#include "WeatherData.h"
#include "broker_database.h"
#include "constants.h"
#include <chrono>
#include <optional>
#include <ratio>
#include <stdexcept>
#include <string>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <cstring>

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
             << bc_broker::version::patch << "\n\n";
}

bool
Broker::dbGood()
{
   return m_database.good();
}

bool
Broker::serialFileGood()
{
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

bool
Broker::setupSerialPort()
{
   int file, rc;
   struct termios options;

   file = open(m_filePath.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
   if (file < 0)
   {
      return false;
   }
   m_fd = file;

   // Set baud rate to 115200
   cfsetispeed(&options, B115200);
   cfsetospeed(&options, B115200);

   // Input settings
   options.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IXON | IXOFF | IXANY);
   options.c_oflag &= ~OPOST;
   options.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
   options.c_cflag &= ~(CSIZE | PARENB);
   options.c_cflag |= (CS8 | CLOCAL | CREAD);

   options.c_cc[VMIN] = 0;
   options.c_cc[VTIME] = 0;

   rc = tcsetattr(file, TCSANOW, &options);

   if (rc == 0)
   {
      m_serialUp = true;
      return true;
   }
   else
   {
      return false;
   }
}

bool
Broker::writeToPort(const std::string &val)
{
   if (!m_serialUp)
   {
      if (!setupSerialPort())
      {
         return false;
      }
   }

   if (val.size() <= 0)
   {
      return false;
   }

   size_t txNum = write(m_fd, val.c_str(), val.size());

   return txNum == val.size();
}

std::string
Broker::readFromPort()
{
   char buf[128] = {0};

   if (!m_serialUp)
   {
      if (!setupSerialPort())
      {
         return "";
      }
   }

   ssize_t rxNum = read(m_fd, buf, sizeof(buf));

   return std::string(buf, strlen(buf));
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

bool Broker::writeToDb(const MessageResponse& response) {

   return m_database.postStreamData(response.m_id, response.m_data);
}

void
Broker::runTasks()
{
   // Try to receive from buffer, and print to standard out
   std::string rcv = readFromPort();
   std::optional<WeatherData> weatherData;

   if (rcv.size() > 0)
   {
      std::cout << "[DEBUG] " << rcv;
      std::optional<MessageResponse> response = MessageParse::parseMessage(rcv);
      if (response)
      {
         std::cout << "[DEBUG] Parsed weather data: T:" << response->m_data.m_temp_c
                   << " H:" << response->m_data.m_humid << " P:" << response->m_data.m_pressure_kpa
                   << " R:" << response->m_data.m_isRaining << " L:" << response->m_data.m_lightLevel << "\n";
         std::cout << "[DEBUG] Network Data ID: " << response->m_id << " RSSI:" << response->m_rssi
                   << " SNR:" << response->m_snr << "\n";

         if (!writeToDb(response.value())){
            std::cerr << "[ERROR] Failed to write to database\n";
         }
      }
   }
}

Broker::~Broker()
{
   // Close file
   close(m_fd);
}
