#include "file_size.h"
#include <iomanip>
#include <iostream>
#include <sys/stat.h>

#define RED "\033[1;31m"
#define NC "\033[0m"

void print_file_size(const std::string &file_name)
{
  struct stat st;
  if (stat(file_name.c_str(), &st) == 0)
  {
    long long size = st.st_size;
    const char *units[] = {"B", "KB", "MB", "GB", "TB"};
    int unit_index = 0;

    while (size >= 1024 && unit_index < sizeof(units) / sizeof(units[0]) - 1)
    {
      size /= 1024;
      unit_index++;
    }

    std::cout << RED << "\n┌─────────────────────────────⬤ \n│ ⬤  File size: "
              << std::fixed << std::setprecision(2) << static_cast<double>(size)
              << " " << units[unit_index]
              << "\n└─────────────────────────────⬤ \n"
              << NC;
  }
  else
  {
    perror((RED + std::string("\n\nCould not get file size") + NC).c_str());
  }
}
