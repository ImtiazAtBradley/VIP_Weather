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
   Broker broker = Broker(brokerDb, std::chrono::duration<int, std::milli>(100), argv[1]);

   // Print program header
   Broker::printProgramHeader();

   // SETUP SERIAL FILE
   
   if (!broker.serialFileGood() || !broker.setupSerialPort()){
      std::cerr << "INVALID FILE INPUT. SEE USAGE:\n\n";
      Broker::help();
      exit(EXIT_FAILURE);
   }
   
   std::cout << "[INFO] Initialized serial port\n";

   // HACK: Write to radio to set it up
   broker.writeToPort("AT+ADDRESS=1\r\n");

   // TRY TO CONNECT TO REDIS SERVER

   if (!broker.dbGood())
   {
      std::cerr << "Connection to redis DB failed\n";
      std::cerr << "hiredis Error: " << broker.getDbError() << "\n";
      exit(EXIT_FAILURE);
   }
   std::cout << "[INFO] Successfully connected to database\n";

   while(1) {
      broker.runScheduler();
   }
}

