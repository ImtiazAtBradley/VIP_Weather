#include <chrono>
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
   BrokerDatabase brokerDb(bc_broker::redis::ip, bc_broker::redis::port, 3);
   // Run broker scheduler every 20ms
   Broker broker = Broker(brokerDb, std::chrono::duration<int, std::milli>(20));

   // Print program header
   broker.printProgramHeader();

   // GET SERIAL PORT FROM USER, IF PRESENT
   if (!broker.validateUserInput(argc, argv))
   {
      std::cout << "INVALID INPUT. SEE USAGE:\n" << std::endl;
      broker.help();
      exit(EXIT_FAILURE);
   }

   // ! TESTING: REMOVEME
   std::cout << "[DEBUG] Fake connecting to serial file... " << argv[1] << "\n";
   // ! END REMOVEME

   // TRY TO CONNECT TO REDIS SERVER

   if (!broker.dbGood())
   {
      std::cerr << "Connection to redis DB failed\n";
      std::cerr << "hiredis Error: " << broker.getDbError();
      exit(EXIT_FAILURE);
   }

   std::cout << "Successfully connected to database\n";

   // WHILE LOOP THAT PERFORMS RX + PUBLISH TASK

   // Clean up and disconnect from database
}
