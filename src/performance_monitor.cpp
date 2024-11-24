#include "performance_monitor.h"

#include <iomanip>
#include <iostream>

static struct timeval start_time;
static struct rusage start_usage;

#define GREEN "\033[1;32m"

void start_monitoring() {
  gettimeofday(&start_time, nullptr);
  getrusage(RUSAGE_SELF, &start_usage);
}

void stop_monitoring() {
  struct timeval end_time;
  struct rusage end_usage;

  gettimeofday(&end_time, nullptr);
  getrusage(RUSAGE_SELF, &end_usage);

  long seconds = end_time.tv_sec - start_time.tv_sec;
  long microseconds = end_time.tv_usec - start_time.tv_usec;
  double elapsed = seconds + microseconds * 1e-6;

  long memory_used = end_usage.ru_maxrss;

  long user_cpu_time =
      end_usage.ru_utime.tv_sec * 1000000 + end_usage.ru_utime.tv_usec;
  long system_cpu_time =
      end_usage.ru_stime.tv_sec * 1000000 + end_usage.ru_stime.tv_usec;
  double cpu_time = (user_cpu_time + system_cpu_time) / 1000000.0;

  const char *border_color = "\033[1;34m";
  const char *reset_color = "\033[0m";

  std::cout << reset_color << "\n┌─────────────────────────────────┐ \n";
  std::cout << "│ " << reset_color << std::left << std::setw(32)
            << "Monitoring Statistics:" << reset_color << "│\n";
  std::cout << "├─────────────────────────────────┘ \n";

  std::cout << reset_color << "│ " << reset_color << std::left << std::setw(15)
            << "Elapsed time:" << border_color;
  if (elapsed < 1.0) {
    std::cout << elapsed * 1000 << " milliseconds";
  } else if (elapsed < 60.0) {
    std::cout << elapsed << " seconds";
  } else if (elapsed < 3600.0) {
    double minutes = elapsed / 60.0;
    std::cout << minutes << " minutes";
  } else if (elapsed < 86400.0) {
    double hours = elapsed / 3600.0;
    std::cout << hours << " hours";
  } else {
    double days = elapsed / 86400.0;
    std::cout << days << " days";
  }
  std::cout << reset_color << "\n";

  std::cout << reset_color << "│ " << reset_color << std::left << std::setw(15)
            << "Memory used:" << border_color;
  if (memory_used < 1024) {
    std::cout << memory_used << " bytes";
  } else if (memory_used < 1024 * 1024) {
    std::cout << memory_used / 1024.0 << " KB";
  } else {
    std::cout << memory_used / (1024.0 * 1024.0) << " MB";
  }
  std::cout << reset_color << "\n";

  std::cout << reset_color << "│ " << reset_color << std::left << std::setw(15)
            << "CPU time:" << border_color;
  if (cpu_time < 1.0) {
    std::cout << cpu_time * 1000 << " milliseconds";
  } else {
    std::cout << cpu_time << " seconds";
  }
  std::cout << reset_color << "\n";
}
