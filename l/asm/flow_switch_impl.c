int
flow_switch_impl(int x, int n)
{
  static void *jt[] = {
    &&LOC_A,
    &&LOC_DEF,
    &&LOC_B,
    &&LOC_C,
    &&LOC_D,
    &&LOC_DEF,
    &&LOC_D
  };

  unsigned index = n - 100;
  int result;

  if (index > 6)
    goto LOC_DEF;

  goto *jt[index]; /* multiway branch */

LOC_DEF:
  result = 0;
  goto DONE;

LOC_C:
  result = x;
  goto REST;

LOC_A:
  result = x * 13;
  goto DONE;

LOC_B:
  result = x + 10;
  /* fall through */

REST:
  result += 11;
  goto DONE;

LOC_D:
  result = x * x;
  /* fall through */

DONE:
  return result;
}

