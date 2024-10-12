#ifndef LOG_READER_H
#define LOG_READER_H

#include <string>
#include <vector>
#include <map> // Добавьте этот заголовок для использования std::map

enum LogLevel
{
    INFO,
    WARNING,
    ERROR,
    DEBUG
};

// Объявление переменной как extern
extern std::map<std::string, LogLevel> logLevelMap;

class LogReader
{
public:
    LogReader(const std::string &logFilePath, const std::vector<LogLevel> &filters, bool realTime);
    void startReading();

private:
    std::string logFilePath;
    std::vector<LogLevel> filters;
    bool realTime;

    void printLog(const std::string &message, LogLevel level);
    bool filterLog(const std::string &line);
};

#endif // LOG_READER_H
