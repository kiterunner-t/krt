int
switch_prob(int x, int n)
{
  int result = x;

  switch (n) {
  case 40:
  case 42:
    result <<= 3;
    break;

  case 43:
    result >>= 3;
    break;

  case 44:
    result = 49 * x * x + 0x11;
    break;

  default:
    result += 0x11;
    break;
  }

  return result;
}

