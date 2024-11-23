#include "log_monitor.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_FILTERS 10
#define MAX_FILTER_LENGTH 256

void print_usage(const char *program_name)
{
    printf("\n");
    printf("\n\033[0;31m");
    printf("▄▄▌         ▄▄ • ▪   ▐ ▄ .▄▄ · ▪   ▄▄ •  ▄ .▄▄▄▄▄▄\n");
    printf("██•  ▪     ▐█ ▀ ▪██ •█▌▐█▐█ ▀. ██ ▐█ ▀ ▪██▪▐█•██  \n");
    printf("██▪   ▄█▀▄ ▄█ ▀█▄▐█·▐█▐▐▌▄▀▀▀█▄▐█·▄█ ▀█▄██▀▐█ ▐█.▪\n");
    printf("▐█▌▐▌▐█▌.▐▌▐█▄▪▐█▐█▌██▐█▌▐█▄▪▐█▐█▌▐█▄▪▐███▌▐▀ ▐█▌·\n");
    printf(".▀▀▀  ▀█▄▀▪·▀▀▀▀ ▀▀▀▀▀ █▪ ▀▀▀▀ ▀▀▀·▀▀▀▀ ▀▀▀ · ▀▀▀ \n");
    printf("\n");
    printf("\n\033[1;33m");
    printf(" 🗲 Usage: %s [-r] [-dp] [-h] [-d <start_date> [end_date]] [-f <level>] -i <file>\n", program_name);
    printf("      -r             Display all changes in real time\n");
    printf("      -f <level>     Level filtering (CRITICAL, WARNING, INFO, "
           "DEBUG)\n");
    printf("      -i <file>      Path to log file\n");
    printf("      -h, --help     Show this help\n");
    printf("      -dp            Don't print log lines\n");
    printf("      -d, --date     Filter by date range\n\033[0m");
}

char *normalize_date(const char *input)
{
    char *normalized = malloc(20 * sizeof(char));
    if (normalized == NULL)
    {
        return NULL;
    }
    struct tm tm = {0};
    if (sscanf(input, "%4d-%2d-%2d %2d:%2d:%2d", &tm.tm_year, &tm.tm_mon, &tm.tm_mday, &tm.tm_hour, &tm.tm_min,
               &tm.tm_sec) == 6)
    {
        tm.tm_year -= 1900;
        tm.tm_mon -= 1;
        strftime(normalized, 20, "%Y-%m-%d %H:%M:%S", &tm);
        return normalized;
    }
    free(normalized);
    return NULL;
}

int main(int argc, char *argv[])
{
    const char *file_name = NULL;
    char *filter_levels[MAX_FILTERS];
    int filter_count = 0;
    int real_time = 0;
    int show_stats = 0;
    int print_lines = 1;
    char *start_date = NULL;
    char *end_date = NULL;

    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-i") == 0 && i + 1 < argc)
        {
            file_name = argv[++i];
        }
        else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0)
        {
            print_usage(argv[0]);
            return EXIT_SUCCESS;
        }
        else if (strcmp(argv[i], "-f") == 0 && i + 1 < argc)
        {
            if (filter_count < MAX_FILTERS)
            {
                filter_levels[filter_count++] = argv[++i];
            }
        }
        else if (strcmp(argv[i], "-r") == 0)
        {
            real_time = 1;
        }
        else if (strcmp(argv[i], "-dp") == 0)
        {
            print_lines = 0;
        }
        else if (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--stats") == 0)
        {
            show_stats = 1;
        }
        else if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--date") == 0)
        {
            if (i + 1 < argc)
            {
                start_date = normalize_date(argv[++i]);
                if (start_date == NULL)
                {
                    fprintf(stderr, "Invalid start date format: %s\n", argv[i]);
                    return EXIT_FAILURE;
                }
                printf("Start date: %s\n", start_date); // Выводим start_date

                if (i + 1 < argc && argv[i + 1][0] != '-')
                {
                    end_date = normalize_date(argv[++i]);
                    if (end_date != NULL)
                    {
                        printf("End date: %s\n", end_date); // Выводим end_date
                    }
                    else
                    {
                        fprintf(stderr, "Invalid end date format: %s\n", argv[i]);
                        return EXIT_FAILURE;
                    }
                }
            }
        }
    }

    if (!file_name)
    {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    start_log_monitor(file_name, filter_levels, filter_count, real_time, show_stats, print_lines, start_date, end_date);

    return EXIT_SUCCESS;
}
