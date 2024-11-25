#include "main.h"
#include "log_monitor.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#define MAX_FILTERS 10

void print_usage(const std::string &program_name) {
  std::cout << "\n";
  std::cout << "\033[0;31m";
  std::cout << "▄▄▌         ▄▄ • ▪   ▐ ▄ .▄▄ · ▪   ▄▄ •  ▄ .▄▄▄▄▄▄\n";
  std::cout << "██•  ▪     ▐█ ▀ ▪██ •█▌▐█▐█ ▀. ██ ▐█ ▀ ▪██▪▐█•██  \n";
  std::cout << "██▪   ▄█▀▄ ▄█ ▀█▄▐█·▐█▐▐▌▄▀▀▀█▄▐█·▄█ ▀█▄██▀▐█ ▐█.▪\n";
  std::cout << "▐█▌▐▌▐█▌.▐▌▐█▄▪▐█▐█▌██▐█▌▐█▄▪▐█▐█▌▐█▄▪▐███▌▐▀ ▐█▌·\n";
  std::cout << ".▀▀▀  ▀█▄▀▪·▀▀▀▀ ▀▀▀▀▀ █▪ ▀▀▀▀ ▀▀▀·▀▀▀▀ ▀▀▀ · ▀▀▀ \n\n";
  std::cout << "               LogInsight 1.0.3 - C++             \n";
  std::cout << "\n";
  std::cout << "\n\033[1;33m";
  std::cout << "Usage: " << program_name
            << " [-r] [-dp] [-h] [-d] [-f <level>] -i <file>\n";
  std::cout << "    -r             Display all changes in real time\n";
  std::cout << "    -f <level>     Level filtering (CRITICAL, WARNING, INFO, "
               "DEBUG)\n";
  std::cout << "    -i <file>      Path to log file\n";
  std::cout << "    -h, --help     Show this help\n";
  std::cout << "    -dp            Don't print log lines\n";
  std::cout << "    -d, --date     Filter by date (YYYY-MM-DD)\n\033[0m";
}

int main(int argc, char *argv[]) {
  std::string file_name;
  std::vector<std::string> filter_levels;
  std::string start_date, end_date;
  bool real_time = false;
  bool show_stats = false;
  bool print_lines = true;

  for (int i = 1; i < argc; i++) {
    if (std::string(argv[i]) == "-i" && i + 1 < argc) {
      file_name = argv[++i];
    } else if (std::string(argv[i]) == "-h" ||
               std::string(argv[i]) == "--help") {
      print_usage(argv[0]);
      return EXIT_SUCCESS;
    } else if (std::string(argv[i]) == "-f" && i + 1 < argc) {
      if (filter_levels.size() < MAX_FILTERS) {
        filter_levels.push_back(argv[++i]);
      }
    } else if (std::string(argv[i]) == "-r") {
      real_time = true;
    } else if (std::string(argv[i]) == "-dp") {
      print_lines = false;
    } else if (std::string(argv[i]) == "-s" ||
               std::string(argv[i]) == "--stats") {
      show_stats = true;
    } else if ((std::string(argv[i]) == "-d" ||
                std::string(argv[i]) == "--date") &&
               i + 1 < argc) {
      std::string date_arg = argv[++i];
      if (date_arg.find('-') != std::string::npos) {
        std::istringstream date_stream(date_arg);
        std::getline(date_stream, start_date, '-');
        std::getline(date_stream, end_date, '-');
      } else {
        start_date = date_arg;
      }
    }
  }

  if (file_name.empty()) {
    print_usage(argv[0]);
    return EXIT_FAILURE;
  }

  start_log_monitor(file_name, filter_levels, real_time, show_stats,
                    print_lines, start_date, end_date);

  return EXIT_SUCCESS;
}
