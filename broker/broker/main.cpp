#include <cerrno>
#include <chrono>
#include <cstring>
#include <iostream>
#include <ratio>
#include <sys/stat.h>
#include <hiredis/hiredis.h>
#include <cstdlib>
#include "broker.h"
#include "broker_database.h"
#include "constants.h"

// ============================ MAIN PROGRAM ============================

int
main(int argc, char *argv[])
{
   if (argv[1] == nullptr || argc != 2){
      std::cout << "INVALID INPUT. SEE USAGE:\n\n";
      Broker::help();
      exit(EXIT_FAILURE);
   }

   BrokerDatabase brokerDb(bc_broker::redis::ip, bc_broker::redis::port, 3);
   // Run broker scheduler every 20ms
   Broker broker = Broker(brokerDb, std::chrono::duration<int, std::milli>(20), argv[1]);

   // Print program header
   Broker::printProgramHeader();

   // SETUP SERIAL FILE
   
   if (!broker.serialFileGood() || !broker.setupSerialPort()){
      std::cerr << "INVALID FILE INPUT. SEE USAGE:\n\n";
      Broker::help();
      exit(EXIT_FAILURE);
   }
   
   std::cout << "[INFO] Initialized serial port\n";

   // TRY TO CONNECT TO REDIS SERVER

   if (!broker.dbGood())
   {
      std::cerr << "Connection to redis DB failed\n";
      std::cerr << "hiredis Error: " << broker.getDbError() << "\n";
      exit(EXIT_FAILURE);
   }
   std::cout << "Successfully connected to database\n";

   // Do a test write
   for (size_t i = 0; i < 50; ++i){
      broker.writeToPort("Some long string to put on the port, and hopefully I will see this");
   }
}

