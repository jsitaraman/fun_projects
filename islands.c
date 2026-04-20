#include<stdio.h>
#include<stdlib.h>

void paintGrid(char **grid, int *flag, int i, int j, int gridSize, int gridColSize) {
    if ( j == -1 || j == gridColSize || i == -1 || i == gridSize )
        return;
    if (grid[i][j]=='0' || flag[i*gridColSize+j]) return;
    //grid[i][j]='0';
    flag[i*gridColSize+j]=1;
    paintGrid(grid,flag,i,j+1,gridSize,gridColSize);
    paintGrid(grid,flag,i,j-1,gridSize,gridColSize);
    paintGrid(grid,flag,i+1,j,gridSize,gridColSize);
    paintGrid(grid,flag,i-1,j,gridSize,gridColSize);
}
int numIslands(char** grid, int gridSize, int* gridColSize) {
    int islandCount=0;
    int csize=gridColSize[0];
    int *flag = (int *)calloc(gridSize*csize,sizeof(int));
    for(int i=0;i<gridSize;i++) {
        for (int j=0;j<csize;j++)
        {
            if (grid[i][j]=='1' && !flag[i*csize+j]) {
                paintGrid(grid,flag,i,j,gridSize,csize);
                islandCount++;
            }
        }
    }
    free(flag);
    return islandCount;
}

int numIslands2(char **grid, int gridSize, int *gridColSize)
{
  int islandCount=0;
  int csize=gridColSize[0];
  int *stack = (int *)malloc(sizeof(int)*csize*gridColSize[0]);
  int top=0;
  for(int i=0;i<gridSize;i++)
   for(int j=0;j<csize;j++) {
    if (grid[i][j]=='1') {
      int top=0;
      stack[0]=i*csize+j;
      int end=1;
      int newend=1;
      islandCount++;
      grid[i][j]='0';
      while(top < end) {
         for(int t=top;t< end;t++) {
            int ii=stack[t]/csize;
            int jj=stack[t]%csize;
            for(int d=0;d<4;d++) {
              int ip=(d < 2) ? 2*(d%2)-1 :0;
              int jp=(d >=2) ? 2*(d%2)-1:0;
              if (ii+ip < 0) continue;
              if (ii+ip > gridSize-1) continue;
              if (jj+jp < 0) continue;
              if (jj+jp > csize-1) continue;
	      if (grid[ii+ip][jj+jp]=='1') {
               stack[newend]=(ii+ip)*csize+jj+jp;
               grid[ii+ip][jj+jp]='0';
               newend++;
             }
           }
         }
         top=end;
         end=newend;
      }
   }
  }
  free(stack);
return islandCount;
}


void main() {
  char **grid;
  grid = (char **)malloc(sizeof(char *)*4);
  for(int i=0;i<4;i++)
    grid[i]=(char *)malloc(sizeof(char)*5);

  grid[0][0]='1';grid[0][1]='1';grid[0][2]='1';grid[0][3]='1';grid[0][4]='0';
  grid[1][0]='1';grid[1][1]='1';grid[1][2]='0';grid[1][3]='1';grid[1][4]='0';
  grid[2][0]='1';grid[2][1]='1';grid[2][2]='0';grid[2][3]='0';grid[2][4]='0';
  grid[3][0]='0';grid[3][1]='0';grid[3][2]='0';grid[3][3]='0';grid[3][4]='0';
  /*
  char grid[4][5]={{'1','1','1','1','0'},
		   {'1','1','0','1','0'},
		   {'1','1','0','0','0'},
 		   {'0','0','0','0','0'}};
  */
 int gridSize=4;
 int gridColSize[4]={5,5,5,5};
 printf("numIslands=%d\n",numIslands2(grid,gridSize,gridColSize));
}
