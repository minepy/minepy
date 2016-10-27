/*
 * $ gcc c_conv_example.c -O3 -Wall ../libmine/mine.c -I../libmine/ -lm
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "mine.h"


int main (int argc, char **argv)
{
  mine_parameter param;
  mine_matrix X, Y;
  mine_pstats *pstats;
  mine_cstats *cstats;

  int i, j, k, z;
  char *ret;


  printf("libmine version %s\n\n", libmine_version);

  /* set the parameters */
  param.alpha = 9;
  param.c = 5;
  param.est = EST_MIC_E;

  /* check the parameters */
  ret = mine_check_parameter(&param);
  if (ret)
    {
      printf("ERROR: %s\n", ret);
      return 1;
    }

  /* build the X matrix */
  X.n = 8;   /* 8 variables */
  X.m = 320; /* 320 samples */
  X.data = (double *) malloc ((X.n * X.m) * sizeof(double));
  for (i=0; i<(X.n * X.m); i++)
    X.data[i] = (double) rand() / (RAND_MAX);

  /* build the Y matrix */
  Y.n = 4;    /* 4 variables */
  Y.m = 320;  /* 320 samples */
  Y.data = (double *) malloc ((Y.n * Y.m) * sizeof(double));
  for (i=0; i<(Y.n * Y.m); i++)
    Y.data[i] = (double) rand() / (RAND_MAX);

  /* compute pairwise statistics between samples in X */
  pstats = mine_compute_pstats(&X, &param);

  if (pstats == NULL)
    {
      printf("ERROR: pstats()\n");
      return 1;
    }

  /* print the TIC matrix */
  printf("     ");
  for (j=1; j<X.n; j++)
    printf("X[%d]  ", j);
  printf("\n");
  k = 0;
  for (i=0; i<X.n-1; i++)
    {
      printf("X[%d] ", i);
      for (z=0; z<i; z++)
        printf("      ");

      for (j=i+1; j<X.n; j++)
        {
          printf("%.3lf ", pstats->tic[k]);
          k++;
        }
      printf("\n");
    }

  /* free pstats */
  free(pstats->mic);
  free(pstats->tic);
  free(pstats);

  printf("\n");

  /* compute statistics between each pair of samples in X and Y */
  cstats = mine_compute_cstats(&X, &Y, &param);

  if (cstats == NULL)
    {
      printf("ERROR: cstats()\n");
      return 1;
    }

  /* print the TIC matrix */
  printf("     ");
  for (j=0; j<Y.n; j++)
    printf("Y[%d]  ", j);
  printf("\n");
  for (i=0; i<X.n; i++)
    {
      printf("X[%d] ", i);
      for (j=0; j<Y.n; j++)
        printf("%.3lf ", cstats->tic[(i*Y.n)+j]);
      printf("\n");
    }

  /* free cstats */
  free(cstats->mic);
  free(cstats->tic);
  free(cstats);

  /* free data */
  free(X.data);
  free(Y.data);

  return 0;
}
