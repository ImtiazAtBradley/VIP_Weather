/**
 * @file test.cpp
 * @author Jacob Simeone (jsimeone@mail.bradley.edu)
 * @brief Testing for broker application - this is dead simple, no unit testing lib or anything
 * 
 */

#include <iostream>

#include "broker.h"
#include "WeatherData.h"

#define API_TEST ("API Test")

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

    const WeatherData wd = WeatherData(20.5f, 85.3f, 998.4f, true, LightLevel::SUNNY);

    bool rc = Broker::PostToAPI(wd, "https://weather.jacobsimeone.net/api/envdata", "fake-api-key");

    PrintTestResult(API_TEST, rc, "No error desc");
}

int main()
{
    std::cout << "Starting Tests\n";

    ApiTest();
}