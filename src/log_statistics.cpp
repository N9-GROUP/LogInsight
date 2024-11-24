#include "log_statistics.h"
#include <iomanip>
#include <iostream>

struct LogLevel
{
  std::string label;
  long int count;
  const char *color;
};

extern long int critical_count;
extern long int warning_count;
extern long int info_count;
extern long int error_count;
extern long int debug_count;
extern long int trace_count;
extern long int unknown_count;
extern long int fatal_count;

void print_statistics()
{
  LogLevel log_levels[] = {
      {"CRITICAL", critical_count, "\033[1;31m"}, // RED
      {"ERROR", error_count, "\033[1;31m"},       // RED
      {"FATAL", fatal_count, "\033[38;5;214m"},   // ORANGE
      {"WARNING", warning_count, "\033[1;33m"},   // YELLOW
      {"INFO", info_count, "\033[1;32m"},         // GREEN
      {"TRACE", trace_count, "\033[1;34m"},       // BLUE
      {"DEBUG", debug_count, "\033[1;32m"},       // GREEN
      {"UNKNOWN", unknown_count, "\033[1;37m"}    // WHITE
  };

  const char *border_color = "\033[1;34m";
  const char *reset_color = "\033[0m";

  std::cout << "\n"
            << reset_color << "┌──────────────────────────────┐ \n";
  std::cout << "│ " << reset_color << "Log Statistics:              " << reset_color << "│\n";
  std::cout << "├────────────┬─────────────────┤ \n";
  std::cout << "│ " << reset_color << std::setw(10) << "Level" << reset_color << " │ "
            << std::setw(15) << "Count" << " │\n";
  std::cout << "├────────────┼─────────────────┤ \n";

  for (const auto &log_level : log_levels)
  {
    if (log_level.count > 0)
    {
      std::cout << "│ " << log_level.color
                << std::setw(10) << log_level.label << reset_color << " │ " << log_level.color << std::setw(15) << log_level.count << reset_color << " │ "
                << reset_color << "\n";
    }
  }

  std::cout << reset_color << "└────────────┴─────────────────┘" << "\n";
}

void display_statistics()
{
  print_statistics();
}
