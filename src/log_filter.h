#ifndef LOG_FILTER_H
#define LOG_FILTER_H

#include <stddef.h>

int parse_date(const char *date_str, struct tm *date);

int should_print_log(const char *line, char *filter_levels[], int filter_count, const char *start_date_str,
                     const char *end_date_str);

#endif // LOG_FILTER_H
