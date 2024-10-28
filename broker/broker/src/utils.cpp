#include <chrono>
#include <sys/stat.h>
#include <stdint.h>

int64_t GetUnixTimestamp()
{
    auto now = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
}


/*
 * Source:
 * https://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exists-using-standard-c-c11-14-17-c
 */
bool FileExists(const char *fileName)
{
   struct stat buffer;
   return (stat(fileName, &buffer) == 0);
}