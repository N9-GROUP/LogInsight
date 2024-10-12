#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
#include "log_reader.h" // Добавлено для доступа к LogLevel

void printHelp();
std::vector<LogLevel> parseFilters(int argc, char *argv[], int &index);
std::string getCurrentTime();

#endif // UTILS_H
