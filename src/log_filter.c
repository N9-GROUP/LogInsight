#include <ctype.h>
#include <regex.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAX_PATTERN_LENGTH 256
#define MAX_FILTER_LENGTH 256

// Функция для разбора даты в формате "YYYY-MM-DD HH:MM:SS"
int parse_date(const char *date_str, struct tm *date)
{
    return sscanf(date_str, "%4d-%2d-%2d %2d:%2d:%2d", &date->tm_year, &date->tm_mon, &date->tm_mday, &date->tm_hour,
                  &date->tm_min, &date->tm_sec) == 6;
}

int should_print_log(const char *line, char *filter_levels[], int filter_count, const char *start_date_str,
                     const char *end_date_str)
{
    struct tm start_date = {0}, end_date = {0}, log_date = {0};
    int has_start_date = 0, has_end_date = 0;

    if (start_date_str && parse_date(start_date_str, &start_date))
    {
        start_date.tm_year -= 1900;
        start_date.tm_mon -= 1;
        has_start_date = 1;
    }
    if (end_date_str && parse_date(end_date_str, &end_date))
    {
        end_date.tm_year -= 1900;
        end_date.tm_mon -= 1;
        has_end_date = 1;
    }

    char log_date_str[20];
    if (sscanf(line, "%19s", log_date_str) == 1)
    {
        parse_date(log_date_str, &log_date);
        log_date.tm_year -= 1900;
        log_date.tm_mon -= 1;
    }
    else
    {
        return 0;
    }

    if (has_start_date || has_end_date)
    {
        if (has_start_date && difftime(mktime(&log_date), mktime(&start_date)) < 0)
        {
            return 0;
        }
        if (has_end_date && difftime(mktime(&log_date), mktime(&end_date)) > 0)
        {
            return 0;
        }
    }

    if (filter_count == 0)
    {
        return 1;
    }

    for (int j = 0; j < filter_count; j++)
    {
        char filter_level_lower[MAX_FILTER_LENGTH];
        size_t filter_length = strlen(filter_levels[j]);

        for (size_t i = 0; i < filter_length; i++)
        {
            filter_level_lower[i] = tolower((unsigned char)filter_levels[j][i]);
        }
        filter_level_lower[filter_length] = '\0';

        char pattern[MAX_PATTERN_LENGTH];

        int written = snprintf(pattern, sizeof(pattern), "\\|\\s*%s\\s*\\|", filter_level_lower);

        if (written < 0 || (size_t)written >= sizeof(pattern))
        {
            fprintf(stderr, "Error formatting regex pattern.\n");
            return 0;
        }

        regex_t regex;
        int reti = regcomp(&regex, pattern, REG_EXTENDED | REG_ICASE);

        if (reti)
        {
            fprintf(stderr, "Could not compile regex\n");
            return 0;
        }

        reti = regexec(&regex, line, 0, NULL, 0);

        regfree(&regex);

        if (reti == 0)
        {
            return 1;
        }
    }

    return 0;
}
