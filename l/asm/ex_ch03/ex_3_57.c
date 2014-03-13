int
cread(int *xp)
{
  int t = 0;
  xp = (xp ? xp : &t);
  return *xp;
}

