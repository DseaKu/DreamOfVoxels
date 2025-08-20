#ifndef PERFORMANCE_H
#define PERFORMANCE_H

#include "std_includes.h"
#include <time.h>

#define MAX_PERFORMANCE_TRACKERS 100

typedef struct {
    const char* name;
    clock_t start_time;
    clock_t end_time;
    double elapsed_time;
    double total_elapsed_time;
    int runs;
} PerformanceTracker;

void StartPerformanceTracker(const char* name);
void EndPerformanceTracker(const char* name);
void PrintPerformanceTrackers(void);
void WritePerformanceTrackersToFile(const char* filename);

#endif // PERFORMANCE_H