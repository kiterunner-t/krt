#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>


void test_time(char *title, struct timeval *start, struct timeval *end);


int
main(int argc, char **argv)
{
  int is_fsync = 0;
  int is_unlink = 0;
  int num = 10000;
  struct timeval start;
  struct timeval end;
  char *file = "test.txt";
  int fd;
  char buf[4096] = "hello krt\n";
  int i;
  ssize_t n;

  switch (argc) {
  case 4: is_unlink = atoi(argv[3]);
  case 3: is_fsync = atoi(argv[2]);
  case 2:
    num = atoi(argv[1]);
    break;

  default : /* do nothing ... */ ;
  }

  fd = open(file, O_CREAT | O_RDWR, 0666);
  if (fd < 0) {
    printf("open error: %s\n", strerror(errno));
    exit(-1);
  }

  if (is_unlink)
    unlink(file);

  gettimeofday(&start, NULL);
  for (i = 0; i < num; ++i) {
    n = write(fd, buf, strlen(buf));
    if (n < 0) {
      printf("write error: %s\n", strerror(errno));
      unlink("test.txt");
      exit(1);
    }
  }
  gettimeofday(&end, NULL);
  test_time("write", &start, &end);

  if (is_fsync) {
    gettimeofday(&start, NULL);
    fsync(fd);
    gettimeofday(&end, NULL);
    test_time("fsync", &start, &end);
  }

  gettimeofday(&start, NULL);
  close(fd);
  gettimeofday(&end, NULL);
  test_time("close", &start, &end);
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


/*
krt@krt:~$ ./a.out 1000000 1 1
----------------write
start: 1402112558  791409
end: 1402112563  38156
4  246747

----------------fsync
start: 1402112563  38416
end: 1402112563  994657
0  956241

----------------close
start: 1402112563  994825
end: 1402112564  2224
0  7399

*/

/*
krt@krt:~$ ./a.out 1000000 0 1
----------------write
start: 1402112575  652316
end: 1402112581  126372
5  474056

----------------close
start: 1402112581  126783
end: 1402112581  876041
0  749258

*/

