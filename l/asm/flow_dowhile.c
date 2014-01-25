int
fact_to(int n)
{
  int result = 1;

  do {
    result *= n;
    n -= 1;
  } while (n > 1);

  return result;
}

