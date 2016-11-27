/*
 * $ gcc-6 -Wall -pg c_prof.c  ../libmine/mine.c -I../libmine/ -lm
 * $ ./a.out
 * $ gprof a.out gmon.out > analysis.txt
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "mine.h"


int main (int argc, char **argv)
{
  mine_problem prob;
  mine_parameter param;
  mine_score *score;

  int i;
  char *ret;


  /* set the parameters */
  param.alpha = 0.6;
  param.c = 5;
  param.est = EST_MIC_E;

  /* check the parameters */
  ret = mine_check_parameter(&param);
  if (ret)
    {
      printf("ERROR: %s\n\n", ret);
      return 1;
    }

  /* build the problem */
  prob.n = 10001;
  prob.x = (double *) malloc (prob.n * sizeof (double));
  prob.y = (double *) malloc (prob.n * sizeof (double));

  for (i=0; i<prob.n; i++)
    {
      prob.x[i] =  (double) rand() / (RAND_MAX);
      prob.y[i] =  (double) rand() / (RAND_MAX);
    }

  /* compute score */
  clock_t start = clock();
  score = mine_compute_score(&prob, &param);
  clock_t end = clock();
  printf("Elapsed time: %.6f seconds\n\n",
         (double)(end - start) / CLOCKS_PER_SEC);

  if (score == NULL)
    {
      printf("ERROR: mine_compute_score()\n");
      return 1;
    }

  /* free score */
  mine_free_score(&score);

  /* free prob */
  free(prob.x);
  free(prob.y);

  return 0;
}
