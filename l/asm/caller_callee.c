void
sfact_helper(long x, long *resultp)
{
  if (x <= 0) {
    *resultp = 1;

  } else {
    long nresult;
    sfact_helper(x - 1, &nresult);
    *resultp = x * nresult;
  }
}


long
sfact(long x)
{
  long result;
  sfact_helper(x, &result);
  return result;
}

