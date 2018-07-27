#include <stdio.h>

#define DISPATCH(x) do { pc= x; goto *dispatch_table[pc]; } while (0)

int pc;

int main()
{
static void* dispatch_table[] =
{
  &&label_1, 
  &&label_2
};

DISPATCH(1);


label_1:
  printf("Label1\n");
  DISPATCH(1);

label_2:
  printf("Label2\n");
  DISPATCH(0);

  return 0;
}
