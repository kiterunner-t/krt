#define N 16


int
array2_prod_ele(int ma[N][N], int mb[N][N], int i, int k)
{
  int j;
  int result = 0;

  for (j = 0; j < N; ++j)
    result += ma[i][j] * mb[j][k];

  return result;
}


/*
 * 利用访问模式的规律优化代码:
 * 1. 引入局部变量访问矩阵ma第i行，访问矩阵mb第k列
 * 2. mb中k列的访问间隔sizeof(int) * N个字节
 */
int
array2_prod_ele_opt(int ma[N][N], int mb[N][N], int i, int k)
{
  int *row_a = &ma[i][0];
  int *col_b = &mb[0][k];
  int  result = 0;
  int  j;

  for (j = 0; j != N; ++j) {
    result += row_a[j] * *col_b;
    col_b += N;
  }

  return result;
}


void
array2_set_diag(int ma[N][N], int val)
{
  int i;

  for (i = 0; i < N; i++)
    ma[i][i] = val;
}


void
array2_set_diag_opt(int ma[N][N], int val)
{
  int *base_a = &ma[0][0];
  int  i = 0;

  do {
    base_a[i] = val;
    i += (N + 1);
  } while (i != (N+1)*N);
}

