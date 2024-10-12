#include "log_reader.h"
#include "utils.h"
#include <iostream>

int main(int argc, char *argv[])
{
    if (argc == 1 || (argc == 2 && (std::string(argv[1]) == "-h" || std::string(argv[1]) == "--help")))
    {
        printHelp();
        return 0;
    }

    std::string logFilePath;
    bool realTime = false;

    int index = 1;
    std::vector<LogLevel> filters;

    while (index < argc)
    {
        if (std::string(argv[index]) == "-i")
        {
            logFilePath = argv[++index];
        }
        else if (std::string(argv[index]) == "-f")
        {
            filters = parseFilters(argc, argv, index);
        }
        else if (std::string(argv[index]) == "-r")
        {
            realTime = true;
        }
        // Обработка других флагов можно добавить здесь...

        ++index;
    }

    LogReader reader(logFilePath, filters, realTime);
    reader.startReading();

    return 0;
}
