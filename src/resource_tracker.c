#include "resource_tracker.h"
#include <stdio.h>
#include <stdlib.h>
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
      trackers[index].name = strdup(name);
      if (trackers[index].name == NULL) {
        tracker_count--; // Roll back
        fprintf(stderr,
                "Error: Failed to allocate memory for tracker name '%s'.\n",
                name);
        return;
      }
      trackers[index].total_elapsed_time = 0.0;
      trackers[index].runs = 0;
    } else {
      fprintf(stderr,
              "Error: Maximum number of performance trackers (%d) reached. "
              "Cannot add '%s'.\n",
              MAX_PERFORMANCE_TRACKERS, name);
      return;
    }
  }
  clock_gettime(CLOCK_MONOTONIC, &trackers[index].start_time);
}

void EndPerformanceTracker(const char *name) {
  int index = FindTracker(name);
  if (index != -1) {
    clock_gettime(CLOCK_MONOTONIC, &trackers[index].end_time);
    trackers[index].elapsed_time =
        (trackers[index].end_time.tv_sec - trackers[index].start_time.tv_sec) +
        (trackers[index].end_time.tv_nsec - trackers[index].start_time.tv_nsec) /
            1e9;
    trackers[index].total_elapsed_time += trackers[index].elapsed_time;
    trackers[index].runs++;
  }
}

static void GeneratePerformanceReport(FILE *stream) {
  fprintf(stream, "\n\n--- Performance Report ---\n");
  for (int i = 0; i < tracker_count; i++) {
    if (trackers[i].runs > 0) {
      fprintf(stream, " %.3f ms (avg over %d runs):%s\n",
              (trackers[i].total_elapsed_time * 1000) / trackers[i].runs,
              trackers[i].runs, trackers[i].name);
    }
  }

  // Search for "CompleteLoop" and print average fps
  fprintf(stream, "\n--- Average FPS after INIT---\n");
  int found = 0;
  for (int i = 0; i < tracker_count; i++) {
    // Use strcmp to compare the current tracker's name with "CompleteLoop"
    if (strcmp(trackers[i].name, "CompleteLoop") == 0) {
      fprintf(stream, " %.3f Average fps\n",
              (1 / (float)(trackers[i].total_elapsed_time / trackers[i].runs)));
      found = 1;
      break;
    }
  }

  if (!found) {
    fprintf(stream, " Tracker 'CompleteLoop' not found.\n");
  }
  fprintf(stream, "--------------------------\n");
}

void PrintPerformanceTrackers(void) { GeneratePerformanceReport(stdout); }

void WritePerformanceTrackersToFile(const char *filename) {
  FILE *file = fopen(filename, "w");
  if (file == NULL) {
    perror("Error opening file for performance report");
    return;
  }

  GeneratePerformanceReport(file);
  fclose(file);
}

void CleanupPerformanceTrackers(void) {
  for (int i = 0; i < tracker_count; i++) {
    free(trackers[i].name);
    trackers[i].name = NULL;
  }
  tracker_count = 0;
}
