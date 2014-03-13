int
exchange(int *p, int y)
{
  int x;

  x = *p;
  *p = y;
  return x;
}

