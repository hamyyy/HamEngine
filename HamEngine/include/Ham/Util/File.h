#pragma once

#include "Ham/Core/Log.h"

#include <iostream>
#include <fstream>
#include <string>

namespace Ham::File {
static std::string Read(const std::string &filePath)
{
  if (filePath.empty())
    return "";

  std::ifstream file(filePath);
  if (!file) {
    HAM_CORE_ERROR("Failed to open file: {0}", filePath);
    return "";
  }

  return std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
}
}  // namespace Ham::File