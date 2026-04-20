#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void permute(char *a, char *s, int n, int idx) {
  int c, i, j, useflag;
  if (n == 1) {
    printf("%s\n", s);
    return;
  }
  for (i = 0; i < n; i++) {
    if (i > 0) {
      useflag=1;
      for(j=0;j<i && useflag; j++)
	 if (a[j]==a[i]) useflag=0;
      if (!useflag) continue;
      c = a[0]; a[0] = a[i]; a[i] = c;
    }
    s[idx] = a[0];
    permute(&a[1], s, n - 1, idx + 1);
    if (i > 0) {
      c = a[0]; a[0] = a[i]; a[i] = c;
    }
  }
}

void main(int argc, char *argv[]) {
  int i;
  int n = strlen(argv[1]);
  permute(argv[1], argv[1], n, 0);
}
