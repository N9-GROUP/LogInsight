#ifndef LOG_MONITOR_H
#define LOG_MONITOR_H

#include <string>
#include <vector>
#include <pthread.h>

void start_log_monitor(const std::string &file_name, const std::vector<std::string> &filter_levels,
                       bool real_time, bool show_stats, bool print_lines);
void process_line(const std::string &line, const std::vector<std::string> &filter_levels, bool print_lines);
void count_log_levels(const std::string &line);

extern pthread_mutex_t count_mutex;

extern long int critical_count;
extern long int warning_count;
extern long int info_count;
extern long int error_count;
extern long int debug_count;
extern long int trace_count;
extern long int unknown_count;
extern long int fatal_count;

#endif // LOG_MONITOR_H
