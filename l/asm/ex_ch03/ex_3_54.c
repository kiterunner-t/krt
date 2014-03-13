int
decode2(int x, int y, int z)
{
  int d = z - y;
  int t = d;
  t <<= 15;
  t >>= 15;
  return (d^x) * t;
}

