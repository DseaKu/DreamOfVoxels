#include "resource_tracker.h"
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
      trackers[index].total_elapsed_time = 0.0;
      trackers[index].runs = 0;
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
    trackers[index].total_elapsed_time += trackers[index].elapsed_time;
    trackers[index].runs++;
  }
}

void PrintPerformanceTrackers(void) {
  printf("\n\n--- Performance Report ---\n");
  for (int i = 0; i < tracker_count; i++) {
    if (trackers[i].runs > 0) {
      printf(" %.3f ms (avg over %d runs):%s\n",
             (trackers[i].total_elapsed_time * 1000) / trackers[i].runs,
             trackers[i].runs, trackers[i].name);
    }
  }

  // Search for "CompleteLoop" and print average fps
  printf("\n--- Average FPS after INIT ---\n");
  int found = 0;
  for (int i = 0; i < tracker_count; i++) {
    // Use strcmp to compare the current tracker's name with "CompleteLoop"
    if (strcmp(trackers[i].name, "CompleteLoop") == 0) {
      printf(" %.3f Average fps\n",
             (1 / (float)(trackers[i].total_elapsed_time / trackers[i].runs)));
      found = 1;
      break;
    }
  }

  if (!found) {
    printf(" Tracker 'CompleteLoop' not found.\n");
  }
  printf("--------------------------\n");
}

void WritePerformanceTrackersToFile(const char *filename) {
  FILE *file = fopen(filename, "w");
  if (file == NULL) {
    perror("Error opening file for performance report");
    return;
  }

  fprintf(file, "\n\n--- Performance Report ---\n");
  for (int i = 0; i < tracker_count; i++) {
    if (trackers[i].runs > 0) {
      fprintf(file, " %.3f ms (avg over %d runs):%s\n",
              (trackers[i].total_elapsed_time * 1000) / trackers[i].runs,
              trackers[i].runs, trackers[i].name);
    }
  }

  // Search for "CompleteLoop" and print average fps
  fprintf(file, "\n--- Average FPS after INIT ---\n");
  int found = 0;
  for (int i = 0; i < tracker_count; i++) {
    // Use strcmp to compare the current tracker's name with "CompleteLoop"
    if (strcmp(trackers[i].name, "CompleteLoop") == 0) {
      fprintf(file, " %.3f Average fps\n",
              (1 / (float)(trackers[i].total_elapsed_time / trackers[i].runs)));
      found = 1;
      break;
    }
  }

  if (!found) {
    fprintf(file, " Tracker 'CompleteLoop' not found.\n");
  }
  fprintf(file, "--------------------------\n");

  fclose(file);
}
