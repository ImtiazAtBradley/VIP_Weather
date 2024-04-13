#pragma once

#include "broker_database.h"

#include <string>

class Broker
{
 public:
   Broker(BrokerDatabase &brokerDb);
   Broker(BrokerDatabase &&brokerDb);
   void programHeader();
   void help();
   bool validateUserInput(int argc, char *argv[]);
   std::string getDbError();
   bool dbGood();

 private:
   bool fileExists(const std::string &name);
   BrokerDatabase m_database;
};

