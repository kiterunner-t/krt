void
f_continue(void)
{
  int sum = 0;
  int i;

  for (i = 0; i < 10; ++i) {
    if (i & 1)
      continue;

    sum += i;
  }
}

