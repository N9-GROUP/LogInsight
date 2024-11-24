#ifndef LOG_FILTER_H
#define LOG_FILTER_H

#include <string>
#include <vector>

bool should_print_log(const std::string &line, const std::vector<std::string> &filter_levels);

#endif // LOG_FILTER_H
