typedef struct stu_s stu_t;

struct stu_s {
  char *name;
  short sex;
};


stu_t
stu_alloc(char *n, short a)
{
  stu_t b;
  b.name = n;
  b.sex = a;
  return b;
}


void
te_stu()
{
  stu_t a = stu_alloc("krt", 1);
}


long long
ll_alloc()
{
  long long a = 8;
  return a;
}


void
te_longlong()
{
  long long ll = ll_alloc();
}


double
d_alloc()
{
  double d = 1.0;
  return d;
}


void
te_double()
{
  double d = d_alloc();
}

