#ifndef PERFORMANCE_H
#define PERFORMANCE_H

#include <time.h>

#define MAX_PERFORMANCE_TRACKERS 100

typedef struct {
  char *name;
  struct timespec start_time;
  struct timespec end_time;
  double elapsed_time;
  double total_elapsed_time;
  int runs;
} PerformanceTracker;

void StartPerformanceTracker(const char *name);
void EndPerformanceTracker(const char *name);
void PrintPerformanceTrackers(void);
void WritePerformanceTrackersToFile(const char *filename);
void CleanupPerformanceTrackers(void);

#endif // PERFORMANCE_H
