#include "log_color.h"
#include <iostream>
#include <regex>

#define RED "\033[0;31m"
#define YELLOW "\033[1;33m"
#define GREEN "\033[0;32m"
#define BLUE "\033[0;34m"
#define PURPLE "\033[0;35m"
#define WHITE "\033[1;37m"
#define ORANGE "\033[38;5;214m"
#define NC "\033[0m"

void colorize_log(const std::string &line)
{
  const std::vector<std::pair<std::regex, const char *>> patterns = {
      {std::regex("\\|\\s*CRITICAL\\s*\\|", std::regex::icase), RED},
      {std::regex("\\|\\s*WARNING\\s*\\|", std::regex::icase), YELLOW},
      {std::regex("\\|\\s*INFO\\s*\\|", std::regex::icase), GREEN},
      {std::regex("\\|\\s*DEBUG\\s*\\|", std::regex::icase), BLUE},
      {std::regex("\\|\\s*ERROR\\s*\\|", std::regex::icase), RED},
      {std::regex("\\|\\s*UNKNOWN\\s*\\|", std::regex::icase), WHITE},
      {std::regex("\\|\\s*TRACE\\s*\\|", std::regex::icase), PURPLE},
      {std::regex("\\|\\s*FATAL\\s*\\|", std::regex::icase), ORANGE}};

  for (const auto &pattern : patterns)
  {
    if (std::regex_search(line, pattern.first))
    {
      std::cout << pattern.second << line << NC << std::endl;
      return;
    }
  }

  std::cout << line
            << std::endl;
}
