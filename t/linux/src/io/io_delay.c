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
  int             is_fsync = 0;
  int             is_unlink = 0;
  int             num = 10000;
  struct timeval  start;
  struct timeval  end;
  char           *file = "test.txt";
  int             fd;
  char            buf[4096] = "hello krt\n";
  int             i;
  ssize_t         n;

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

  if (is_unlink) {
    unlink(file);
    printf("    unlink\n");
  }
  if (is_fsync) {
    printf("    fsync\n");
  }
  printf("\n");

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
  rename(file, "test_another.txt");
  gettimeofday(&end, NULL);
  test_time("rename", &start, &end);

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

krt@krt:~/work/github/krt/t/linux/src/io$ ./a.out 1000000 1
    fsync

----------------write
start: 1402147737  817798
end: 1402147743  76873
5  259075

----------------fsync
start: 1402147743  77026
end: 1402147743  635889
0  558863

----------------rename
start: 1402147743  636036
end: 1402147743  642558
0  6522

----------------close
start: 1402147743  642645
end: 1402147743  642677
0  32

*/

/*

krt@krt:~/work/github/krt/t/linux/src/io$ ./a.out 1000000 1 1
    unlink
    fsync

----------------write
start: 1402147768  448894
end: 1402147774  907808
6  458914

----------------fsync
start: 1402147774  908141
end: 1402147774  981000
0  72859

----------------rename
start: 1402147774  986021
end: 1402147774  986080
0  59

----------------close
start: 1402147774  986122
end: 1402147774  991844
0  5722

*/

