#include "log_filter.h"
#include <cctype>
#include <iostream>
#include <regex>

bool should_print_log(const std::string &line,
                      const std::vector<std::string> &filter_levels)
{
  if (filter_levels.empty())
  {
    return true;
  }

  for (const auto &filter_level : filter_levels)
  {
    std::string filter_level_lower;
    for (char c : filter_level)
    {
      filter_level_lower += std::tolower(static_cast<unsigned char>(c));
    }

    std::string pattern = "\\|\\s*" + filter_level_lower + "\\s*\\|";
    std::regex regex_pattern(pattern, std::regex::icase);

    if (std::regex_search(line, regex_pattern))
    {
      return true;
    }
  }

  return false;
}
