#include "utils.h"

#include <chrono>
#include <sys/stat.h>
#include <stdint.h>
#include <fstream>
#include <time.h>

#include <iostream>

int64_t
GetUnixTimestamp()
{
   auto now = std::chrono::system_clock::now();
   return std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
}

// Source: https://stackoverflow.com/questions/9527960/how-do-i-construct-an-iso-8601-datetime-in-c
std::string GetIso8601Time()
{
    time_t now;
    time(&now);
    char buf[sizeof "2011-10-08T07:07:09Z"];
    strftime(buf, sizeof buf, "%FT%TZ", gmtime(&now));
}

/*
 * Source:
 * https://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exists-using-standard-c-c11-14-17-c
 */
bool
FileExists(const std::string fileName)
{
   struct stat buffer;
   return (stat(fileName.c_str(), &buffer) == 0);
}

/**
 * @brief Get content of file
 * 
 * @return std::optional<std::string>
 */
std::optional<std::string>
GetFileContent(std::string filePath)
{
    if (!FileExists(filePath))
    {
        return std::nullopt;
    }

    std::ifstream fs(filePath);
    std::string key((std::istreambuf_iterator<char>(fs)), (std::istreambuf_iterator<char>()));

    return key;
}