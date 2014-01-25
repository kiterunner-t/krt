int
callee(int *xp, int *yp)
{
  int x = *xp;
  int y = *yp;

  *xp = y;
  *yp = x;
  return x + y;
}


int
caller(void)
{
  int arg1 = 534;
  int arg2 = 1057;
  int sum;
  int diff;

  sum = callee(&arg1, &arg2);
  diff = arg1 - arg2;
  return sum * diff;
}

