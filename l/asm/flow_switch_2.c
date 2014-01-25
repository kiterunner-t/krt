int
flow_switch_2(int n)
{
  int result = 0;

  switch (n) {
  case 1:
    result = n + 1;
    break;

  case 10:
    result = n + 10;
    break;

  case 100:
    result = n + 100;
    break;

  default:
    result = n;
    break;
  }

  return result;
}

