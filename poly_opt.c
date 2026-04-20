#include<stdio.h>
#include<stdlib.h>
int kth_permutation(int n, int * src, int k, int * dst) {
    int avail[10];
    for(int i=0;i<n;i++) avail[i]=src[i];
    for (int i = 0; i < n-1; i++)
        for (int j = i+1; j < n; j++)
            if (avail[i] > avail[j]) {
                char t = avail[i]; avail[i] = avail[j]; avail[j] = t;
            }

    // Check k is in range
    int f = 1;
    for (int i = 2; i <= n; i++) f *= i;
    if (k < 0 || k >= f) return -1;

    int remaining = n;
    for (int i = 0; i < n; i++) {
        f /= remaining;           // f is now (remaining-1)!
        int digit = k / f;
        k %= f;
        dst[i] = avail[digit];
        // remove avail[digit] from pool
        for (int j = digit; j < remaining-1; j++)
            avail[j] = avail[j+1];
        remaining--;
    }
  return 0;
}

#define swap(a, b)                                                             \
  {                                                                            \
    a = a + b;                                                                 \
    b = a - b;                                                                 \
    a = a - b;                                                                 \
  }

/* create a simple heuristic for filling */

void markFaces(int **bins, int npoly, int nfaces) {
  int totalFaces = npoly*nfaces;
  if (totalFaces < 9) {
   printf("Not enough faces to carry all numbers\n");
   exit(0);
  }
  int junk;
  // we have to make the first row all zeros anyway
  // otherwise single digits cannot be made
  for(int c=0;c<npoly;c++) bins[0][c]=0;
  int done=0;
  int *counts=(int *)malloc(sizeof(int)*7);
  for(int i=0;i<7;i++) counts[i]=npoly;
  int imod=7;
  while(!done) {
    int m=0;
    int value=1;
    int v=0;
    int c8=0;
    for(int f=1;f < nfaces; f++) {
      for(int c=0;c<npoly;c++) {
	bins[f][c]=value;
	if (value==8) c8++;
	v++;
	if (v==counts[value-1]) {
	  value++;
	  v=0;	  
	}
      }
    }
    //printf("%d %d %d %d %d %d %d\n",counts[0],counts[1],counts[2],counts[3],counts[4],counts[5],counts[6]);
    //printf("c8=%d imod=%d\n",c8,imod);
    //getchar();
    if (c8 < npoly-2 || c8==0) {
      imod--;
      if (imod < 0) imod=6;
      counts[imod]--;
    } else {
      done=1;
    }
  }
  free(counts);
}

void main()
{
  int *perm0, *perm, *pos, *col;
  int **bins;
  int npoly,nfaces;
  
  printf("NPOLY: ");
  scanf("%d",&npoly);
  printf("FACES_PER_POLY: ");
  scanf("%d",&nfaces);
  bins=(int **)malloc(sizeof(int *)*nfaces);
  for(int j=0;j<nfaces;j++)
    bins[j]=(int *)malloc(sizeof(int)*npoly);
  perm0=(int *)malloc(sizeof(int)*npoly);
  perm=(int *)malloc(sizeof(int)*npoly);
  pos=(int *)malloc(sizeof(int)*npoly);
  col=(int *)malloc(sizeof(int)*npoly);

  markFaces(bins,npoly,nfaces);
  
  for (int j = 0; j < npoly; j++) {
    printf("cube[%d]=", j);
    for (int k = 0; k < nfaces; k++)
      printf("%d ", bins[k][j]);
    printf("\n");
    perm[j]=perm0[j]=j;
  }

  int nfact=1; 
  for(int j=2;j<=npoly;j++) nfact=nfact*j;  
  int i=1;
  int k=0;
  while(1) {
    i++;
    int m=0;
    int found;
    while(m < nfact) {
      int val=i;
      for(int c=0;c<npoly;c++) {
	found=0;
	int ip=perm[c];
        //printf("%d %d %d\n",perm[0],perm[1],perm[2]); 
	for(int f=0;f<nfaces;f++) {
	  if (val % 10 == bins[f][ip]) found=1;
	  if (val % 10 == 9 && bins[f][ip]==6) found=1;
          //printf("\t v: cube %d face: %d val: %d %d %d\n",ip, f, val%10,bins[f][ip], found);
	  if (found) {
            pos[c]=ip;
            col[c]=f; 
            break;
          }
	}
	if (!found) break;
	val=val/10;
      }      
      if (found) break;
      m++;
      k=(k+1)%nfact;
      kth_permutation(npoly,perm0,k,perm);
    }
    if (m==nfact && !found) break;
    printf("%d :", i);
    for (int k = npoly - 1; k >= 0; k--)
      printf("%d", pos[k]);
    printf(": ");
    for (int k = npoly - 1; k >= 0; k--)
      printf("%d", bins[col[k]][pos[k]]);
    printf("\n");
  }
  printf("i=%d\n",i-1);

  for(int j=0;j<nfaces;j++) free(bins[j]);
  free(bins);

  free(perm0);
  free(perm);
  free(pos);
  free(col);

}
 
