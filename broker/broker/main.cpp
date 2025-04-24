#include <cerrno>
#include <cstring>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <regex>
#include <sys/stat.h>
#include <cstdlib>

#include "utils.h"
#include "broker.h"
#include "constants.h"
#include "wslogger.h"

// ============================ MAIN PROGRAM ============================

static void
FailOut()
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

   // Try to get URL from command line arguments
   if (argv[2] == nullptr)
   {
      std::cerr << "MISSING: You must provide an API URL\n";
      FailOut();
   }

   std::string url = argv[2]; // Kind of trusting here this was a valid URL

   // Try to get the API key from the provided file

   if (argv[3] == nullptr)
   {
      std::cerr << "MISSING: You must provide a key file" << std::endl;
      FailOut();
   }

   std::string keyFilePath = argv[3];

   std::optional<std::string> key = GetFileContent(keyFilePath);

   if (!key)
   {
      std::cerr << "Invalid key file path\n";
      FailOut();
   }

   // Hacky, but works for our purposes - check for ascii only in the future too
   auto check_whitespace = [](char x) { return x == ' ' || x == '\n' || x == '\r' || x == '\t'; };
   bool contains_whitespace = std::find_if(key.value().begin(), key.value().end(), check_whitespace) != key.value().end();

   if (key == "")
   {
      std::cerr << "Key file is empty" << std::endl;
      FailOut();
   }
   else if (contains_whitespace)
   {
      std::cerr << "Key file contains invalid whitespace, did you accidentally leave some?" << std::endl;
      std::cerr << "Got api key: \"" << key.value() << "\"\n";
      FailOut();
   }

   // Run broker scheduler every 20ms
   Broker broker =
       Broker(std::chrono::milliseconds(1000), std::string(argv[1]), url, key.value());

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

   std::cout << "Starting Broker...\n";

   while (1)
   {
      broker.runScheduler();
   }
}
