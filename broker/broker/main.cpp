#include <iostream>
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
   std::cout << "HELLO, WORLD!\n" << std::endl;

   BrokerDatabase brokerDb(bc_broker::redis::ip, bc_broker::redis::port);
   Broker broker = Broker(brokerDb);

   // Print program header
   broker.programHeader();

   // GET SERIAL PORT FROM USER, IF PRESENT
   // TODO: Make sure to remove the "or true" which is for debugging
   if (!broker.validateUserInput(argc, argv) || true)
   {
      std::cout << "INVALID INPUT. SEE USAGE:\n" << std::endl;
      broker.help();
      exit(EXIT_FAILURE);
   }

   std::cout << "Fake connecting to serial file... " << argv[1] << "\n";

   // TRY TO CONNECT TO REDIS SERVER

   if (broker.dbGood())
   {
      std::cerr << "Connection to redis DB failed\n";
      std::cerr << "hiredis Error: " << broker.getDbError();
      exit(EXIT_FAILURE);
   }

   std::cout << "Successfully connected to database\n";

   // WHILE LOOP THAT PERFORMS RX + PUBLISH TASK

   // Clean up and disconnect from database
}
