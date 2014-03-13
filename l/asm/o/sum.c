int accum = 0;

int
sum(int x, int y)
{
  int t;

  t = x + y;
  accum += t;
  return t;
}

