#include<stdio.h>
#include<math.h>

void main(int argc, char *argv[])
{
 FILE *fp;
 char s[53];
 int i;
 fp=fopen(argv[1],"r");
 fgets(s,53,fp);
 double pow2=0.5;
 double val=1.0;
 for(i=1;i<=52;i++) {
   int bit=(s[i-1]=='0')?0:1;
   val+=bit*pow2;
   pow2/=2.0;
 }
 printf("%.16lf\n",val); //*pow(2.0,1021-1023));

}
