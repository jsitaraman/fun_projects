#include <stdio.h>
#define swap(a, b)  { a=a+b;b=a-b;a=a-b;}

void generate(int n, char* p) {
    int c[n];
    for (int i = 0; i < n; i++) c[i] = 0;

    // Print initial
    for (int i = 0; i < n; i++) printf("%c", p[i]);
    printf("\n");

    int i = 0;
    while (i < n) {
        if (c[i] < i) {
            if (i % 2 == 0) {
                swap(p[0], p[i]);   // swap first and i-th
              }
            else {
                swap(p[c[i]], p[i]); // swap c[i]-th and i-th
             }
            
            for (int j = 0; j < n; j++) printf("%c", p[j]);
            printf("\n");

            c[i]++;
            i = 0;
        } else {
            c[i] = 0;
            i++;
        }
    }
}

int main() {
    char p[] = {'a', 'b', 'c'};
    generate(3, p);
}
