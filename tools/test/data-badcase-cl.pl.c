/*
 * this is a bad multiline comment, if drop the next line
 */
void
main()
{
  printf("this is a /* in */ quote\n");
  printf("this is a test in /* test\n");
  printf("another test in */ test\n");
  printf("this another /*\
test for\
another\n");
  printf("this is\
a nother /* test\
for comment\
so */ what's your option?\n");
}


void
test()
{
  printf("test /*\" */");
}

