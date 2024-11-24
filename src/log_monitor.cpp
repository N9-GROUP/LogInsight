#include "log_monitor.h"
#include "file_size.h"
#include "log_color.h"
#include "log_filter.h"
#include "log_statistics.h"
#include "performance_monitor.h"
#include <csignal>
#include <cstring>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <sys/inotify.h>
#include <sys/select.h>
#include <unistd.h>

#define INITIAL_BUFFER_SIZE 1024
#define EVENT_SIZE (sizeof(struct inotify_event))
#define EVENT_BUF_LEN (1024 * EVENT_SIZE)

long int critical_count = 0;
long int warning_count = 0;
long int info_count = 0;
long int debug_count = 0;
long int error_count = 0;
long int trace_count = 0;
long int unknown_count = 0;
long int fatal_count = 0;

std::vector<std::regex> regex_patterns(8);
static bool running = true;
pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;

void handle_signal(int signal)
{
    if (signal == SIGINT)
    {
        running = false;
        std::cout << "\033[0;31m\n\nBye...\n\033[0m";
    }
}

void compile_regex_patterns()
{
    const std::string patterns[] = {
        "\\|\\s*CRITICAL\\s*\\|", "\\|\\s*WARNING\\s*\\|",
        "\\|\\s*INFO\\s*\\|", "\\|\\s*DEBUG\\s*\\|",
        "\\|\\s*ERROR\\s*\\|", "\\|\\s*UNKNOWN\\s*\\|",
        "\\|\\s*TRACE\\s*\\|", "\\|\\s*FATAL\\s*\\|"};

    for (size_t i = 0; i < sizeof(patterns) / sizeof(patterns[0]); i++)
    {
        regex_patterns[i] = std::regex(patterns[i], std::regex::icase);
    }
}

void count_log_levels(const std::string &line)
{
    pthread_mutex_lock(&count_mutex);

    bool matched = false;

    for (size_t i = 0; i < regex_patterns.size(); i++)
    {
        if (std::regex_search(line, regex_patterns[i]))
        {
            matched = true;
            switch (i)
            {
            case 0:
                critical_count++;
                break; // CRITICAL
            case 1:
                warning_count++;
                break; // WARNING
            case 2:
                info_count++;
                break; // INFO
            case 3:
                debug_count++;
                break; // DEBUG
            case 4:
                error_count++;
                break; // ERROR
            case 5:
                unknown_count++;
                break; // UNKNOWN
            case 6:
                trace_count++;
                break; // TRACE
            case 7:
                fatal_count++;
                break; // FATAL
            }
            break;
        }
    }

    if (!matched)
    {
        unknown_count++;
    }

    pthread_mutex_unlock(&count_mutex);
}

void process_line(const std::string &line,
                  const std::vector<std::string> &filter_levels,
                  bool print_lines)
{
    if (should_print_log(line, filter_levels))
    {
        if (print_lines)
        {
            colorize_log(line);
            count_log_levels(line);
        }
        else
        {
            count_log_levels(line);
        }
    }
    else
    {
        count_log_levels(line);
    }
}

void start_log_monitor(const std::string &file_name,
                       const std::vector<std::string> &filter_levels,
                       bool real_time, bool show_stats, bool print_lines)
{
    std::signal(SIGINT, handle_signal);
    start_monitoring();

    compile_regex_patterns();
    if (show_stats)
    {
        // start_monitoring();
    }

    std::ifstream file(file_name);
    if (!file.is_open())
    {
        std::cerr << "Error: " << file_name << std::endl;
        return;
    }

    std::string line;
    while (running && std::getline(file, line))
    {
        process_line(line, filter_levels, print_lines);
        if (real_time)
        {
            std::cout << line << std::endl;
        }
    }

    file.close();
    if (show_stats)
    {
        display_statistics();
        print_file_size(file_name);
    }

    if (real_time)
    {
        int fd = inotify_init();
        if (fd < 0)
        {
            std::cerr << "Error inotify" << std::endl;
            return;
        }

        int wd = inotify_add_watch(fd, file_name.c_str(), IN_MODIFY);
        if (wd < 0)
        {
            std::cerr << "Error: " << file_name
                      << std::endl;
            close(fd);
            return;
        }

        char buffer[EVENT_BUF_LEN];
        while (running)
        {
            int length = read(fd, buffer, EVENT_BUF_LEN);
            if (length < 0)
            {
                std::cerr << "Error inotify" << std::endl;
                break;
            }

            for (int i = 0; i < length;)
            {
                struct inotify_event *event = (struct inotify_event *)&buffer[i];
                if (event->mask & IN_MODIFY)
                {
                    std::ifstream file(file_name);
                    while (std::getline(file, line))
                    {
                        process_line(line, filter_levels, print_lines);
                    }
                    file.close();
                }
                i += EVENT_SIZE + event->len;
            }
        }

        inotify_rm_watch(fd, wd);
        close(fd);
    }

    if (show_stats)
    {
        // stop_monitoring();
    }
}
