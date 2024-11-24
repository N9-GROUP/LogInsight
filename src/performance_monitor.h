#ifndef PERFORMANCE_MONITOR_H
#define PERFORMANCE_MONITOR_H

#include <iostream>
#include <sys/resource.h>
#include <sys/time.h>

void start_monitoring();
void stop_monitoring();

#endif // PERFORMANCE_MONITOR_H
