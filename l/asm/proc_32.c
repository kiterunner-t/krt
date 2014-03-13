int
callee(int *px, int *py)
{
  int x = *px;
  int y = *py;
  
  *px = y;
  *py = x;
  return x + y;
}


int
caller()
{
  int arg1 = 534;
  int arg2 = 1057;
  int sum;
  int diff;
  
  sum = callee(&arg1, &arg2);
  diff = arg1 - arg2;
  return sum * diff;
}

