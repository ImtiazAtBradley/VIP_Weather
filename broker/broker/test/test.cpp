/**
 * @file test.cpp
 * @author Jacob Simeone (jsimeone@mail.bradley.edu)
 * @brief Testing for broker application - this is dead simple, no unit testing lib or anything
 * 
 */

#include <iostream>
#include <optional>

#include "broker.h"
#include "WeatherData.h"
#include "utils.h"

#define API_TEST ("API Test")
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
    // API Test
    PrintTestHeader(API_TEST);

    WeatherData wd = WeatherData(20.5f, 85.3f, 998.4f, true, LightLevel::SUNNY);

    bool rc = Broker::PostToAPI(wd, "https://weather.jacobsimeone.net/api/envdata", "fake-api-key");

    PrintTestResult(API_TEST, rc, "No error desc");
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

    FileReadTest();
}