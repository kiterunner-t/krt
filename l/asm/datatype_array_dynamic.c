int
var_prod_ele(int n, int a[n][n], int b[n][n], int i, int k)
{
  int j;
  int result = 0;

  for (j = 0; j < n; ++j)
    result += a[i][j] * b[i][j];

  return result;
}

