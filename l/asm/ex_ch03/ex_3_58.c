int
switch3(int *p1, int *p2, int action)
{
  int result = 0;

  switch (action) {
  case 0:
    result = *p1;
    *p1 = *p2;
    *p2 = result;
    break;

  case 1:
    *p2 += *p1;
    result = *p2;
    break;

  case 2:
    *p2 = 15;
    result = *p1;
    break;

  case 3:
    *p2 = *p1;
 
  case 4:
    result = 17;
    break;

  default:
    break;
  }

  return result;
}

