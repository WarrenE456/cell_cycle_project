# pragma once
#include <string>

// File path to source directory
const std::string SOURCE_DIRECTORY = std::string(__FILE__).substr(0, std::string(__FILE__).find_last_of("/\\"));
