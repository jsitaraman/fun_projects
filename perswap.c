#include<stdio.h>
#define swap(a, b)                                                             \
  {                                                                            \
    a = a + b;                                                                 \
    b = a - b;                                                                 \
    a = a - b;                                                                 \
  }

void main()
{
  char p[4]={'a','b','c','d'};
  printf("%d %c%c%c%c\n",0, p[0],p[1],p[2],p[3]);
  for(int i=1;i<6;i++) {
    if (i%6 == 0) {
      swap(p[0],p[3]);
      swap(p[2],p[3]); 
    }
    else if (i%2 ==0) {
      swap(p[1],p[3]);
    }
    else {
      swap(p[2],p[3]);
    }
    printf("%d %c%c%c%c\n",i, p[0],p[1],p[2],p[3]);
  }
}
