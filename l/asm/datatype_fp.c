int
sum(int x, int y)
{
  return x + y;
}


typedef int (*func_fp)(int x, int y);


void
te_fp()
{
  func_fp f = sum;
  f(1, 2);
  sum(1, 2);
}

