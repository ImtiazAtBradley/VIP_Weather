#include <cerrno>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <hiredis/hiredis.h>
#include <cstdlib>
#include "utils.h"
#include "broker.h"
#include "constants.h"

// ============================ MAIN PROGRAM ============================

static void FailOut()
{
   std::cout << "INVALID INPUT. SEE USAGE:\n\n";
   Broker::help();
   exit(EXIT_FAILURE);
}

int
main(int argc, char *argv[])
{
   if (argv[1] == nullptr)
   {
      FailOut();
   }

   // Try to get the API key from the provided file

   if (argv[2] == nullptr)
   {
      std::cerr << "You must provide a key file" << std::endl;
      FailOut();
   }

   std::string keyFilePath = argv[2];

   if (!FileExists(keyFilePath.c_str()))
   {
      std::cerr << "You must provide a valid key file" << std::endl;
      FailOut();
   }

   std::ifstream fs(keyFilePath);
   std::string key((std::istreambuf_iterator<char>(fs)), (std::istreambuf_iterator<char>()));

   if (key == "")
   {
      std::cerr << "Key file is empty" << std::endl;
      FailOut();
   }

   // Run broker scheduler every 20ms
   Broker broker = Broker(std::chrono::duration<int, std::milli>(100), argv[1], bc_broker::api::url, key);

   // Print program header
   Broker::printProgramHeader();

   // SETUP SERIAL FILE

   if (!broker.serialFileGood() || !broker.setupSerialPort())
   {
      std::cerr << "INVALID FILE INPUT. SEE USAGE:\n\n";
      Broker::help();
      exit(EXIT_FAILURE);
   }

   std::cout << "[DEBUG] Initialized serial port\n";

   // HACK: Write to radio to set it up
   broker.writeToPort("AT+ADDRESS=1\r\n");

   // TRY TO CONNECT TO REDIS SERVER
   
   std::cout << "Starting Broker...\n";
  
   while (1)
   {
      broker.runScheduler();
   }
}
