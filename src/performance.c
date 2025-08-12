#include "performance.h"
#include <stdio.h>
#include <string.h>

static PerformanceTracker trackers[MAX_PERFORMANCE_TRACKERS];
static int tracker_count = 0;

static int FindTracker(const char *name) {
  for (int i = 0; i < tracker_count; i++) {
    if (strcmp(trackers[i].name, name) == 0) {
      return i;
    }
  }
  return -1;
}

void StartPerformanceTracker(const char *name) {
  int index = FindTracker(name);
  if (index == -1) {
    if (tracker_count < MAX_PERFORMANCE_TRACKERS) {
      index = tracker_count++;
      trackers[index].name = name;
    } else {
      // Handle error: too many trackers
      return;
    }
  }
  trackers[index].start_time = clock();
}

void EndPerformanceTracker(const char *name) {
  int index = FindTracker(name);
  if (index != -1) {
    trackers[index].end_time = clock();
    trackers[index].elapsed_time =
        ((double)(trackers[index].end_time - trackers[index].start_time)) /
        CLOCKS_PER_SEC;
  }
}

void PrintPerformanceTrackers(void) {
  printf("--- Performance Report ---\n");
  for (int i = 0; i < tracker_count; i++) {
    printf("%s: %f ms\n", trackers[i].name, trackers[i].elapsed_time * 1000);
  }
  printf("--------------------------\n");
}
