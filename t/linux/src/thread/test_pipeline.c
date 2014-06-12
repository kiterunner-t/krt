/**
 * Copyleft (C) KRT, 2013-2014 by kiterunner_t
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>
#include "kmisc.h"
#include "pipeline.h"


static int g_count;
static int g_random_count;


static kerrno_t
_stage_tail(int index, void **data)
{
  g_count++;
  int res = (int) (long) ((long *) *data);
  res += index;
//  printf("    result: %d\n", res);
  *data = (void *) (long) res;
  return KSUCCESS;
}


static kerrno_t
_stage(int index, void **data)
{
  int res = (int) (long) ((long *) *data);
  res += index;
  *data = (void *) (long) res;
  return KSUCCESS;
}


void
test_time(char *title, struct timeval *start, struct timeval *end)
{
  printf("----------------%s\n", title);
  printf("start: %ld  %ld\n", start->tv_sec, start->tv_usec);
  printf("end: %ld  %ld\n", end->tv_sec, end->tv_usec);

  if (end->tv_usec < start->tv_usec) {
    end->tv_sec -= 1;
    end->tv_usec += 1000000;
  }
  printf("%ld  %ld\n\n", end->tv_sec - start->tv_sec, end->tv_usec - start->tv_usec);
}


int
main(int argc, char **argv)
{
  pipeline_t                *pl;
  char                       line[1024];
  int                        n;
  int                        i;
  int                        stages;
  pipeline_stage_process_pf *stages_proc;
  int                        max_random;
  struct timeval             start;
  struct timeval             end;
  int                        start_num = 0;

  stages = atoi(argv[1]);
  max_random = atoi(argv[2]);
  if (argc == 4) {
    start_num = atoi(argv[3]);
    if (start_num <= 0) {
      printf("    usage: test_pipeline <stage_num> <max_random> [start_num]\n");
      return 1;
    }
  }

  if (stages<1 || max_random<1) {
    printf("    usage: test_pipeline <stage_num> <max_random> [start_num]\n");
    return 1;
  }

  stages_proc = malloc(stages * sizeof(pipeline_stage_process_pf));

  for (i = 0; i < stages-1; ++i)
    stages_proc[i] = _stage;
  stages_proc[stages-1] = _stage_tail;

  pl = pipeline_create(stages, stages_proc);
  free(stages_proc);

  if (start_num == 0)
    printf("input an integer, and end with a command named 'q'\n");

  while (1) {
    if (start_num == 0) {
      printf("data> ");
      if (fgets(line, sizeof(line), stdin) == NULL)
        break;

      line[strlen(line)-1] = 0;
      if (strcmp(line, "q") == 0)
        break;

      if (sscanf(line, "%d", &n) != 1) {
        printf("please input valid integer\n");
        continue;
      }
    }

//    int num = random() % max_random;
    g_random_count += max_random;

    gettimeofday(&start, NULL);

    for (i = max_random; i > 0; --i)
      pipeline_start(pl, (void *) (long) (n + i));

    pipeline_wait(pl, 0);

    gettimeofday(&end, NULL);
    test_time("pipeline", &start, &end);
    if (start_num != 0)
      break;
  }

  printf("\n\n---------------count is: %d[%d]\n\n", g_count, g_random_count);
  assert(g_count == g_random_count);

  pipeline_wait(pl, 1);
  pipeline_destroy(pl);
  return EXIT_SUCCESS;
}

