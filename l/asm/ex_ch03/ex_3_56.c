int
loop(int x, int n)
{
  int result = 0x55555555;
  int mask;

  for (mask = 0x80000000; mask != 0; mask = result >> n)
    result ^= mask + n;

  return result;
}

