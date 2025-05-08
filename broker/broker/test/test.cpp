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

void ApiTest()
{
    bool rc = true;

    // API Test
    PrintTestHeader(API_TEST);

    std::string rxStr = "+RCV=0,27,T22.72|H37.07|P100.35|G10.3|R1234|L4321,-60,37";
    std::optional<MessageResponse> res = MessageParse::parseMessage(rxStr);
    
    if (!res)
    {
        rc = false;
    }
    else
    {
        rc = Broker::PostToAPI(res->m_data, "http://localhost:27500/api/envdata", "fake-api-key");
    }

    PrintTestResult(API_TEST, rc, "No error desc");
}

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

    // Makes a network call - only use this test if absolutely necessary
    // ApiTest();

    // Perform logging test
    LoggingTest();

    // FileReadTest();
}
