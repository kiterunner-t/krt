/**
 * Copyleft (C) KRT, 2013 by kiterunner_t
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kmisc.h"
#include "pipeline.h"


int
main(int argc, char **argv)
{
  pipeline_t *pl;
  char        line[1024];
  int         n;

  pl = pipeline_create(10);

  printf("input an integer, and end with a command named 'q'\n");
  while (1) {
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

    if (pipeline_start(pl, (void *) n) != KSUCCESS) {
      printf("pipeline start error\n");
      break;
    }
  }

  pipeline_wait(pl);
  pipeline_destroy(pl);
  return EXIT_SUCCESS;
}

