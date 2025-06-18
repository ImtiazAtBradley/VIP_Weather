/**
 * @file test.cpp
 * @author Jacob Simeone (jsimeone@mail.bradley.edu)
 * @brief Testing for broker application - this is dead simple, no unit testing lib or anything
 * 
 */

#include <iostream>
#include <optional>
#include <sys/stat.h>

#include "curl/curl.h"

#include "broker.h"
#include "wslogger.h"
#include "MessageParse.h"
#include "MessageResponse.h"
#include "WeatherData.h"
#include "constants.h"
#include "utils.h"

#define API_TEST ("API Test")
#define LOG_TEST ("Logging Test Using sbd")
#define FILE_READ_TEST ("File Read Test")

// TEST LIBRARY

void PrintTestHeader(std::string testName)
{
    std::cout << "[Starting Test: " << testName << "] ...\n";
}

void PrintTestResult(std::string testName, bool pass, std::string errDesc = "")
{
    std::cout << "[Test: " << testName << "]";
    if (pass)
    {
        std::cout << " PASSED\n";
    }
    else
    {
        std::cout << " FAILED MSG: [" << errDesc << "]\n";
    }
}

// TESTS =======================================================================

void LoggingTest()
{
    PrintTestHeader(LOG_TEST);
    bool rc = true;

    if (mkdir("./bu-weather-station", 0755) != 0)
    {
        rc = false;
    }
    else
    {
        auto logger = WSLogger("./bu-weather-station/broker.log", 1024 * 10, 5);
        logger.Debug("This is a debug message");
        logger.Info("This is a info message");
        logger.Error("This is a error message");
    }

    PrintTestResult(LOG_TEST, rc, "Potentially failed to create directory");

}

void FileReadTest()
{
    bool rc = false;

    PrintTestHeader(FILE_READ_TEST);

    std::optional<std::string> fileData = GetFileContent("./test-file.keys");

    if (fileData)
    {
        std::cout << "File contains: [" << fileData.value() << "]\n";
        rc = true;
    }

    PrintTestResult(FILE_READ_TEST, rc, "File data optional is null");
}

int main()
{
    std::cout << "Starting Tests\n";

    // Perform logging test
    LoggingTest();

    // FileReadTest();
}
