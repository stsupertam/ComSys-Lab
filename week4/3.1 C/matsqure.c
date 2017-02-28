#include <stdio.h>
static int first[10010][10010], multiply[10010][10010];
int m;

void multi()
{
  int c, d, k, sum = 0;

  for (c = 0; c < m; c++) {
    for (d = 0; d < m; d++) {
      for (k = 0; k < m; k++) {
        sum = sum + first[c][k]*first[k][d];
      }
 
      multiply[c][d] = sum;
      sum = 0;
    }
  }
}
void read(char *in){
	FILE *fp;
 	fp = fopen(in, "r");
 	fscanf(fp,"%d", &m);
 	int i,j;
  for (i = 0; i < m; i++)
    for (j = 0; j < m; j++){
      fscanf(fp,"%d", &first[i][j]);
		}
}
