#include<cstdio>
#include<cmath>
#include<ctime>
#include<cstdlib>
#include<string>

typedef struct csr_matrix {
  double *dvalues;
  int *offsets;
  int *columns;
  int nrows;
  int ncols;
} csr_matrix;

typedef struct tree {
  int nchildren;
  int nodeid;
  struct tree *children;
} tree;


// Generate a random dense symmetric positive definite matrix A = B^T B + nI
double *generate_dense_spd_matrix(int N) {
  double *A = (double *) malloc(sizeof(double)*N*N);
  double *B=(double *) malloc(sizeof(double)*N*N);
  srand((unsigned int)time(NULL));
  for (int i = 0; i < N; i++)
    for (int j = 0; j < N; j++)
      B[i*N+j] = (rand() % N) / (double)N;

    // Compute A = B^T * B + nI to ensure it's SPD
    for (int i = 0; i < N; i++)
      for (int j = 0; j < N; j++) {
	A[i*N+j] = 0.0;
	for (int k = 0; k < N; k++)
	  A[i*N+j] += B[k*N+i] * B[k*N+j];
	if (i == j) A[i*N+j] += N;
      }
    free(B);
    return A;
}

// Convert dense matrix to CSR format
csr_matrix csr_spd_matrix(int N) {
  double *mat = generate_dense_spd_matrix(N);
  csr_matrix A;
  int capacity = 0;
  A.offsets=(int *)malloc((N+1)*sizeof(int));
  int idx = 0;
  A.offsets[0] = 0;
  for (int i = 0; i < N; i++) {
    int ncol=0;
    for (int j = 0; j < N; j++) {
      if (mat[i*N+j] > N*0.3) ncol++;
    }
    A.offsets[i+1]=A.offsets[i]+ncol;
  }
  A.nrows=N;
  A.ncols=N;
  A.columns=(int *)malloc(A.offsets[N]*sizeof(int));
  A.dvalues=(double*)malloc(A.offsets[N]*sizeof(double));
  for(int i=0;i < N;i++) {
    int ncol=0;
    for (int j=0;j< N;j++) {
      if (mat[i*N+j] > N*0.3) {
	A.columns[A.offsets[i]+ncol]=j;
	A.dvalues[A.offsets[i]+ncol]=mat[i*N+j];
	ncol++;
      }
    }
  }
  free(mat);
  return A;
}


  
void create_csr_matrix(int nrows, int ncols, int max_fill, csr_matrix *A)
{
  A->offsets=(int *)malloc(sizeof(int)*(nrows+1));
  A->offsets[0]=0;
  for(int i=0;i<nrows;i++) {
    int row_count = (rand() % (max_fill-1))+1;
    A->offsets[i+1]=A->offsets[i]+row_count;
  }
  A->dvalues = (double *)malloc(sizeof(double)*(A->offsets[nrows]));
  A->columns = (int *)malloc(sizeof(double)*(A->offsets[nrows]));

  int m=0;
  for(int i=0;i<nrows;i++) {
    int nentries = A->offsets[i+1]-A->offsets[i];
    int min = 0;
    int max = (ncols - nentries);    
    int loc = (rand()%(max-min+1)) + min;
    for(int j=A->offsets[i];j<A->offsets[i+1];j++)
      {
	A->dvalues[j] = 1+(double)rand()/(double)RAND_MAX;
        if (j==A->offsets[i]) {
   	   A->columns[j] = loc;
        }
        else {
          min = A->columns[j-1]+1;
          max = (ncols-nentries);
          loc = (rand()%(max-min+1))+min;
          A->columns[j]=loc;
        }
        nentries--;
      }
  }
  A->nrows=nrows;
  A->ncols=ncols;
}

void create_csr_sym_matrix(int nrows, int ncols, int max_fill, csr_matrix *A)
{
  A->offsets=(int *)malloc(sizeof(int)*(nrows+1));
  A->offsets[0]=0;
  for(int i=0;i<nrows;i++) {
    int row_count = (rand() % (max_fill-1))+1;
    A->offsets[i+1]=A->offsets[i]+row_count;
  }
  A->dvalues = (double *)malloc(sizeof(double)*(A->offsets[nrows]));
  A->columns = (int *)malloc(sizeof(double)*(A->offsets[nrows]));

  int m=0;
  for(int i=0;i<nrows;i++) {
    int nentries = A->offsets[i+1]-A->offsets[i];
    int min = 0;
    int max = (ncols - nentries);    
    int loc = (rand()%(max-min+1)) + min;
    for(int j=A->offsets[i];j<A->offsets[i+1];j++)
      {
	A->dvalues[j] = 1+(double)rand()/(double)RAND_MAX;
        if (j==A->offsets[i]) {
   	   A->columns[j] = loc;
        }
        else {
          min = A->columns[j-1]+1;
          max = (ncols-nentries);
          loc = (rand()%(max-min+1))+min;
          A->columns[j]=loc;
        }
        nentries--;
      }
  }
  A->nrows=nrows;
  A->ncols=ncols;
}

void output_matrix(csr_matrix *A)
{
  printf("\n");
  int m=0;
  for(int i=0;i<A->nrows;i++) {
    int col_id = A->offsets[i];
    for(int j=0;j<A->ncols;j++) {
      if (j==A->columns[m] && m < A->offsets[i+1]) {
	printf("%.2f ",A->dvalues[m]);
	m++;
      } else {
	printf("%4d ",0);
      }
    }
    printf("\n");
  }   
  printf("\n");
}

void output_matrix_sparsity(csr_matrix *A)
{
  printf("\n");
  int m=0;
  for(int i=0;i<A->nrows;i++) {
    int col_id = A->offsets[i];
    for(int j=0;j<A->ncols;j++) {
      if (j==A->columns[m] && m < A->offsets[i+1]) {
	printf("%s ","x");
	m++;
      } else {
	printf("%s ","-");
      }
    }
    printf("\n");
  }   
  printf("\n");
}

void transpose_matrix(csr_matrix *A, csr_matrix *B) {
  B->nrows=A->ncols;
  B->ncols=A->nrows;
  B->offsets = (int *)malloc(sizeof(int)*(B->nrows+1));
  B->columns = (int *)malloc(sizeof(int)*A->offsets[A->nrows]);
  B->dvalues = (double *)malloc(sizeof(double)*A->offsets[A->nrows]);
  /* count columns first */
  B->offsets[0]=0;
  for(int i=0;i<A->nrows;i++) {
    for(int j=A->offsets[i];j<A->offsets[i+1];j++) {
        int col_id = A->columns[j];
        /* this has to be an atomic */
        B->offsets[col_id+1]++;
     }
  }
  /* create cumulative index */
  for(int i=0;i<B->nrows;i++) {
    B->offsets[i+1] = B->offsets[i]+B->offsets[i+1];
  }
  int *icount = (int *) malloc(sizeof(int)*B->nrows);
  for(int i=0;i<B->nrows;i++) icount[i]=0;
  /* now fill in the transpose */ 
  for(int i=0;i<A->nrows;i++) {
    for(int j=A->offsets[i];j<A->offsets[i+1];j++) {
      int col_id = A->columns[j];
      B->dvalues[icount[col_id]+B->offsets[col_id]]=A->dvalues[j];
      B->columns[icount[col_id]+B->offsets[col_id]]=i;
      icount[col_id]++;
    }  
  }
  free(icount);
}
// multiply using dense format
csr_matrix matmul_sparse(csr_matrix *A, csr_matrix *B)
{
  csr_matrix C;
  C.nrows=0;
  C.ncols=0;
  // return if matrices are not compatible for 
  // multiplication
  if (A->ncols != B->nrows) return C;
  //
  C.nrows=A->nrows;
  C.ncols=B->ncols;
  C.offsets=(int *)malloc(sizeof(double)*(C.nrows+1));
  C.offsets[0]=0;
  //
  int *colFlag = (int *)malloc(sizeof(int)*B->ncols);
  // first determine the column count in each row of C
  C.offsets[0]=0;
  for(int i=0;i<C.nrows;i++) {
    for(int j=0;j<C.ncols;j++) colFlag[j]=0;
    for(int j=A->offsets[i];j<A->offsets[i+1];j++) {
      int rowB =A->columns[j]; // same as column of A
      for(int k=B->offsets[rowB];k<B->offsets[rowB+1];k++) {
	int col = B->columns[k];
	colFlag[col]=1;
      }
    }
    int ncols=0;
    for(int j=0;j<C.ncols;j++) ncols+=colFlag[j];
    C.offsets[i+1]=C.offsets[i]+ncols;
  }
  // now allocate columns and values array
  C.columns = (int *)malloc(sizeof(int)*C.offsets[C.nrows]);
  C.dvalues = (double *)calloc(C.offsets[C.nrows],sizeof(double));
  // now fill the data t the appropriate spot for
  // each row
  for(int i=0;i<C.nrows;i++) {
    for(int j=0;j<C.ncols;j++) colFlag[j]=-1;
    int colcount=0;
    for(int j=A->offsets[i];j<A->offsets[i+1];j++) {
      int rowB =A->columns[j]; // same as column of A
      for(int k=B->offsets[rowB];k<B->offsets[rowB+1];k++) {
	int col = B->columns[k];
	if (colFlag[col]==-1) {
	  colFlag[col]=colcount;
	  colcount++;
	}	  
      }
    }
    int ncols=0;
    for(int j=0;j<C.ncols;j++) {
       if (colFlag[j]!=-1) { colFlag[j]=ncols++;}
    }
    for(int j=A->offsets[i];j<A->offsets[i+1];j++) {
      int rowB =A->columns[j]; // same as column of A
      double vA = A->dvalues[j];
      for(int k=B->offsets[rowB];k<B->offsets[rowB+1];k++) {
	int col = B->columns[k];
	double vB = B->dvalues[k];
	int vid = C.offsets[i]+colFlag[col];
	C.dvalues[vid]+=(vA*vB);
        C.columns[vid] = col;
      }
    }
  }  
  free(colFlag);
  return C;
}
// multiply using dense format
csr_matrix matmul_dense(csr_matrix *A, csr_matrix *B)
{
  csr_matrix C;
  C.nrows=0;
  C.ncols=0;
  // return if matrices are not compatible for 
  // multiplication
  if (A->ncols != B->nrows) return C;
  //
  C.nrows=A->nrows;
  C.ncols=B->ncols;
  C.dvalues=(double *)malloc(sizeof(double)*(C.nrows*C.ncols));
  C.columns=(int *)malloc(sizeof(int)*(C.nrows*C.ncols));
  C.offsets=(int *)malloc(sizeof(double)*(C.nrows+1));
  C.offsets[0]=0;
  //
  for(int i=0;i<C.nrows;i++) {
    C.offsets[i+1]=C.offsets[i]+C.ncols;
  }
  //
  for(int i=0;i<C.nrows;i++)
    for(int j=0;j<C.ncols;j++) {
      C.columns[i*C.ncols+j]=j;
      C.dvalues[i*C.ncols+j]=0;
    }
  //
  for(int i=0;i<A->nrows;i++) {
    for(int j=A->offsets[i];j<A->offsets[i+1];j++) {
      int rowB =A->columns[j]; // same as column of A
      double vA = A->dvalues[j];
      for(int k=B->offsets[rowB];k<B->offsets[rowB+1];k++) {
	int col = B->columns[k];
	double vB = B->dvalues[k];
	C.dvalues[i*C.ncols+col]+=vA*vB;
      }
    }
  }
  return C;
}

tree find_etree(csr_matrix *A)
{  
  int *parent = (int *)malloc(sizeof(int)*A->nrows);
  for(int i=0;i<A->nrows;i++)
    {
      int initparent = A->ncols;
      for(int j=A->offsets[i];j<A->offsets[i+1];j++)
	{
	  if (A->columns[j] <= i) continue;
          //printf("%d ",A->columns[j]);
	  initparent = (initparent > A->columns[j])?A->columns[j]:initparent;
	}
      //printf("\n");
      parent[i] = (initparent > i && initparent < A->ncols) ? initparent : i;
      //printf("parent[%d]=%d %d\n",i,parent[i],initparent);
    }
  /* find fill-ins */
  for(int i=0;i<A->nrows;i++) {
    for(int j=A->offsets[i];j<A->offsets[i+1];j++) {
      int col1 = A->columns[j];
      if (col1 >i) {
	for(int k=j+1;k<A->offsets[i+1];k++) {
	  int col2=A->columns[k];
	  if (col1 < col2) 
	    parent[col1] = (parent[col1] > col2 || parent[col1]==col1)? col2:parent[col1];
	  if (col2 < col1)
	    parent[col2] = (parent[col2] > col1 || parent[col2]==col2)? col1:parent[col2];
	}
      }
    }
  }
  int *isparent = (int *)calloc(A->nrows,sizeof(int));
  int root;
  for(int i=0;i<A->nrows;i++) {
    if (parent[i] <= A->ncols && parent[i]!=i) {
      isparent[parent[i]]=1;
    }
  }
  int *level = (int *) malloc(A->nrows*sizeof(int));
  for(int l=0;l<A->nrows;l++) level[l]=-1;

  printf("level 0 :");
  for(int i=0;i<A->nrows;i++) {
    if (isparent[i]==0 ) {
      printf("%d ",i);
      level[i]=0;
    }
  }
  printf("\n");
  int l=0;
  int complete=0;
  while(!complete) {
   printf("l=%d : ",l);
   for(int i=0;i<A->nrows;i++) {
     //printf("\tlevel[%d]=%d\n",i,level[i]);
     if (level[i]==l && parent[i]!=i) {
        printf("%d ",i);
        level[parent[i]]=l+1;
        //printf("\t\t%d %d %d %d\n",i,parent[i],level[i],level[parent[i]]);
     }
   }
   printf("\n");
   complete=1;
   for(int i=0;i<A->nrows;i++) {
      if (level[i]==-1) {
        complete=0;
        break;
      }
   }
   if (complete) {
    printf("l=%d : ",l+1);
    for(int i=0;i<A->nrows;i++) {
     if (level[i]==l+1) {
       printf("%d ",i);
     }
    }
    printf("\n");
   }
   l++;
  }
  printf("\n");
 
  printf("\n");
  free(isparent);
  tree T;
  return T;
}

csr_matrix read_csr(const char *fname)
{
  FILE *fp=fopen(fname,"r");
  csr_matrix a;
  int nvals;
  fscanf(fp,"%d %d",&a.nrows,&nvals);
  a.offsets=(int *)malloc(sizeof(int)*(a.nrows+1));
  a.columns=(int *)malloc(sizeof(int)*nvals);
  a.dvalues=(double *)malloc(sizeof(double)*nvals);
  // assume rows=cols for now
  a.ncols=a.nrows;
  for(int i=0;i<=a.nrows;i++) {
    fscanf(fp, "%d ",&a.offsets[i]);
  }
  for(int i=0;i<nvals;i++) {
   fscanf(fp, "%d ", &a.columns[i]);
  }
  for(int i=0;i<nvals;i++) {
   fscanf(fp,"%lf ",&a.dvalues[i]);
  }
  fclose(fp);
  return a;
}


int main()
{
  csr_matrix A,B;
  int nrowsA=10;
  int ncolsA=6;
  int max_fill=3;
  int nrowsB=6;
  int ncolsB=8;

  //output_matrix(&B);
  //transpose_matrix(&A,&B);

  create_csr_matrix(nrowsA,ncolsA,max_fill, &A);
  //output_matrix(&A);
  create_csr_matrix(nrowsB,ncolsB,max_fill, &B);
  //output_matrix(&B);
  csr_matrix C=matmul_dense(&A,&B);
  //output_matrix(&C);

  csr_matrix D=matmul_sparse(&A,&B);
  //output_matrix(&D);

  //csr_matrix E=csr_spd_matrix(19);
  //output_matrix(&E);

  csr_matrix E = read_csr(std::string("csr.mat").c_str());
  output_matrix_sparsity(&E);
  tree T = find_etree(&E);
}
