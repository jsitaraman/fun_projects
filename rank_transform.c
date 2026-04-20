#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define NP 10
#define swap(a,b) {int tmp=a; a=b; b=tmp;}

int **get_matrix(int n, int m)
{
  int **a;
  srand((unsigned int)time(NULL));
  a=(int **)malloc(sizeof(int *)*n);
  for(int i=0;i<n;i++)
    a[i]=(int *)malloc(sizeof(int)*m);

  for(int i=0;i<n;i++)
    for(int j=0;j<m;j++)
      a[i][j]=(rand()%NP);
  return a;
}

void print_matrix(int **a, int n, int m)
{
  for(int i=0;i<n;i++) {
    for(int j=0;j<m;j++)
      printf("%d ",a[i][j]);
    printf("\n");
  }
}

void print_matrix_ll(int **a, int n, int m)
{
  printf("[");
  for(int i=0;i<n;i++) {
    printf("[");
    for(int j=0;j<m;j++) {
      printf("%d%c",a[i][j],(j==m-1)?0:',');      
    }
    printf("]%c",(i==n-1)?0:',');
  }
  printf("]\n");
}

void print_matrix_tab(int **a, int n, int m)
{
  for(int i=0;i<n;i++) {
    printf("\t");
    for(int j=0;j<m;j++)
      printf("%d ",a[i][j]);
    printf("\n");
  }
}


typedef struct {
    int value;
    int index;
} Item;

int cmp_item(const void *p1, const void *p2) {
    const Item *a = (const Item *)p1;
    const Item *b = (const Item *)p2;
    return (a->value > b->value) - (a->value < b->value);
}

int *sort(int *a, int n) {
    Item *arr = (Item *)malloc(sizeof(Item) * n);
    int *ord = (int *)malloc(sizeof(int) * n);
    if (!arr || !ord) {
        free(arr);
        free(ord);
        return NULL;
    }

    // Fill with values and original indices
    for (int i = 0; i < n; i++) {
        arr[i].value = a[i];
        arr[i].index = i;
    }

    // Sort using qsort
    qsort(arr, n, sizeof(Item), cmp_item);

    // Copy back sorted values and store order
    for (int i = 0; i < n; i++) {
        a[i] = arr[i].value;
        ord[i] = arr[i].index;
    }

    free(arr);
    return ord;
}


int * sort_old(int *a, int n)
{
 int *ord = (int *)malloc(sizeof(int)*n);
 for(int i=0;i<n;i++) ord[i]=i;
 for(int i=0;i<n;i++)
  for(int j=i+1;j<n;j++) {
   if (a[i] > a[j]) {
     swap(a[i],a[j]);
     swap(ord[i],ord[j]);
   }
  }
 return ord;
}

int **get_min_row_matrix(int **a,int n,int m)
{
  int **b;
  int *tmp=(int *)malloc(sizeof(int)*m);
  b=(int **) malloc(sizeof(int *)*n);
  for(int i=0;i<n;i++)
   b[i]=(int *)malloc(sizeof(int)*m);
  for(int i=0;i<n;i++) {
   for(int j=0;j<m;j++)
      tmp[j]=a[i][j];
   int *ord = sort(tmp,m);
   int rank=1;
   rank=1;
   b[i][ord[0]]=rank;
   for(int j=1;j<m;j++) {
    if (tmp[j] > tmp[j-1]) rank++;
    b[i][ord[j]]=rank;
   }
   free(ord);
  }
  free(tmp);
  return b;
}

int **get_min_col_matrix(int **a, int n, int m)
{
  int **b;
  int *tmp;
  b=(int **) malloc(sizeof(int *)*n);
  for(int i=0;i<n;i++)
   b[i]=(int *)malloc(sizeof(int)*m);
  tmp=(int *)malloc(sizeof(int)*n);
  for(int j=0;j<m;j++) {
    for(int i=0;i<n;i++)
      tmp[i]=a[i][j];
    int *ord = sort(tmp,n);
    int rank=1;
    b[ord[0]][j]=rank;
    for(int i=1;i<n;i++) {
      if (tmp[i] > tmp[i-1]) rank++;
      b[ord[i]][j]=rank;
    }
    free(ord);
  }
  free(tmp);
  return b;
}


int **max_rank_matrix(int **a, int n, int m)
{
  int **b;
  b=(int **) malloc(sizeof(int *)*n);
  for(int i=0;i<n;i++)
   b[i]=(int *)malloc(sizeof(int)*m);
  int *tmp=(int *)malloc(sizeof(int)*n*m);
  int *tmp2=(int *)malloc(sizeof(int)*n*m);
  for(int i=0;i<n;i++)
    for(int j=0;j<m;j++) 
      tmp[i*m+j]=a[i][j];
  int *ord = sort(tmp,n*m);
  int rank=1;
  tmp2[0]=rank;
  for(int i=1;i<n*m;i++) {
    if (tmp[i] > tmp[i-1]) rank++;
    tmp2[i]=rank;
  }
  for(int i=0;i<n*m;i++) {
    int idx=ord[i];
    int ii=idx/m;
    int jj=idx%m;
    b[ii][jj]=tmp2[i];
  }
  free(tmp);
  free(tmp2);
  free(ord);
  return b;
}

int **rank_transform(int **a, int n,  int m)
{
  int **max_rank=max_rank_matrix(a,n,m);
  int **prev_rank=max_rank_matrix(a,n,m);

  int complete=0;
  int *stack = (int *)malloc(sizeof(int)*n*m);
  int iter=0;
  while(!complete) {
    printf("iter %d\n",++iter);
    print_matrix(max_rank,n,m);

    int top=-1;
    for(int i=0;i<n;i++)
      for(int j=0;j<m;j++) {
	--max_rank[i][j];
	if (max_rank[i][j]==0) {
          max_rank[i][j]++;
	  stack[++top]=i*m+j;
	}
      }
    printf("after sub\n");
    print_matrix(max_rank,n,m);
    while(top >= 0) {
      int idx=stack[top--];
      int row = idx/m;
      int col = idx%m;
      //max_rank[row][col]+=1;
      printf("top:%d (%d %d)\n",top,row+1,col+1);
      print_matrix_tab(max_rank,n,m);
      for(int i=0;i<n;i++) {
	if (i==row) continue;
	if (max_rank[i][col]==max_rank[row][col] && a[i][col] > a[row][col]) {
          max_rank[i][col]++;
	  stack[++top]=i*m+col;
	}
        else if (a[i][col]==a[row][col] && max_rank[i][col]!=max_rank[row][col]) {
          max_rank[i][col]++;
          stack[++top]=i*m+col;
        }
      }
      for(int j=0;j<m;j++) {
	if (j==col) continue;
	if (max_rank[row][j]==max_rank[row][col] && a[row][j] > a[row][col]) {
          max_rank[row][j]++;
	  stack[++top]=row*m+j;
	}
        else if (a[row][j]==a[row][col] && max_rank[row][j]!=max_rank[row][col]) {
          max_rank[row][j]++;
          stack[++top]=row*m+j;
        }
      }
    }
    // check if we have achieved convergence
    complete=1;
    for(int i=0;i<n && complete;i++)
      {
	for(int j=0;j<m;j++) {
	  if (max_rank[i][j] != prev_rank[i][j]) {
	    complete=0;
	    break;
	  }
	}
      }
    if (!complete) {
      for(int i=0;i<n;i++)
	for(int j=0;j<m;j++)
	  prev_rank[i][j]=max_rank[i][j];
    }
  }
  free(stack);
  for(int i=0;i<n;i++) free(prev_rank[i]);
  free(prev_rank);
  return max_rank;
}
  
void main()
{
  int n=4;
  int m=4;
  int **a=get_matrix(n,m);
  
  FILE *fp=fopen("2.dat","r");
  for(int i=0;i<n;i++)
    for(int j=0;j<m;j++)
      fscanf(fp,"%d",&(a[i][j]));
  fclose(fp);
  
  //print_matrix_ll(a,n,m);
  //printf("\n");

  int **rankT=rank_transform(a,n,m);
  //print_matrix(rankT,n,m);
  //print_matrix(max_rank_matrix(a,n,m),n,m);
  /*
  print_matrix(get_min_row_matrix(a,n,m),n,m);
  printf("\n");
  print_matrix(get_min_col_matrix(a,n,m),n,m);
  */
}
