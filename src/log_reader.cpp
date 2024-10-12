#include "log_reader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <thread> // Добавлено
#include <chrono> // Добавлено
#include "utils.h"

std::map<std::string, LogLevel> logLevelMap = {
    {"INFO", INFO},
    {"WARNING", WARNING},
    {"ERROR", ERROR},
    {"DEBUG", DEBUG}};

LogReader::LogReader(const std::string &logFilePath, const std::vector<LogLevel> &filters, bool realTime)
    : logFilePath(logFilePath), filters(filters), realTime(realTime) {}

void LogReader::printLog(const std::string &message, LogLevel level)
{
    switch (level)
    {
    case INFO:
        std::cout << "\033[32m" << message << "\033[0m"; // Green
        break;
    case WARNING:
        std::cout << "\033[33m" << message << "\033[0m"; // Yellow
        break;
    case ERROR:
        std::cout << "\033[31m" << message << "\033[0m"; // Red
        break;
    case DEBUG:
        std::cout << "\033[34m" << message << "\033[0m"; // Blue
        break;
    }
}

bool LogReader::filterLog(const std::string &line)
{
    for (const auto &filter : filters)
    {
        if (line.find(filter == INFO ? "INFO" : filter == WARNING ? "WARNING"
                                            : filter == ERROR     ? "ERROR"
                                                                  : "DEBUG") != std::string::npos)
        {
            return true;
        }
    }
    return false;
}

void LogReader::startReading()
{
    std::ifstream logFile(logFilePath);

    if (!logFile.is_open())
    {
        std::cerr << "Не удалось открыть файл: " << logFilePath << std::endl;
        return;
    }

    std::string line;

    while (true)
    {
        while (std::getline(logFile, line))
        {
            LogLevel level = INFO; // Default level
            if (line.find("INFO") != std::string::npos)
                level = INFO;
            else if (line.find("WARNING") != std::string::npos)
                level = WARNING;
            else if (line.find("ERROR") != std::string::npos)
                level = ERROR;
            else if (line.find("DEBUG") != std::string::npos)
                level = DEBUG;

            if (filterLog(line))
            {
                printLog(line + "\n", level);
            }
        }

        if (!realTime)
            break;

        // Wait for new logs
        logFile.clear();                                      // Clear EOF flag
        logFile.seekg(0, std::ios_base::end);                 // Move to end of file
        std::this_thread::sleep_for(std::chrono::seconds(1)); // Poll every second
        logFile.clear();
    }
}
