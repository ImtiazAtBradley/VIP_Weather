#pragma once

#include <stdint.h>
#include <string>
#include <optional>

int64_t GetUnixTimestamp();

bool FileExists(const std::string fileName);

std::optional<std::string> GetFileContent(std::string filePath);
