#include "utils.h"
#include <iostream>
#include <map>
#include <ctime>
#include <iomanip>
#include <sstream>
#include "log_reader.h"

// Определение переменной здесь
std::map<std::string, LogLevel> logLevelMap = {
    {"info", INFO},
    {"warning", WARNING},
    {"error", ERROR},
    {"debug", DEBUG}};

void printHelp()
{
    std::cout << "Использование: ./log_reader -i <path_to_log_file> [-f <filters>] [-r] [-d <date>] [-o <output_file>] [-h]\n";
}

std::vector<LogLevel> parseFilters(int argc, char *argv[], int &index)
{
    std::vector<LogLevel> filters;

    while (++index < argc && argv[index][0] != '-')
    {
        auto it = logLevelMap.find(argv[index]);
        if (it != logLevelMap.end())
        {
            filters.push_back(it->second);
        }
    }

    --index; // Adjust the index after the inner loop
    return filters;
}

std::string getCurrentTime()
{
    auto now = std::time(nullptr);
    std::ostringstream oss;
    oss << std::put_time(std::localtime(&now), "%Y-%m-%d %H:%M:%S");
    return oss.str();
}
