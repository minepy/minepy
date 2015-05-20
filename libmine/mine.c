/*  
    libmine core library
    
    This code is written by Davide Albanese <davide.albanese@gmail.com>
    and Michele Filosi <filosi@fbk.eu>.
    Copyright (C) 2012 Davide Albanese, Copyright (C) 2012 Michele 
    Filosi, Copyright (C) 2012 Fondazione Bruno Kessler.

    References:
    a) D. Reshef, Y. Reshef, H. Finucane, S. Grossman, G. McVean,
       P. Turnbaugh, E. Lander, M. Mitzenmacher, P. Sabeti. 
       Detecting novel associations in large datasets.
       Science 334, 6062 (2011)
    b) D. Albanese, M. Filosi, R. Visintainer, S. Riccadonna, G. Jurman,
       C. Furlanello.
       minerva and minepy: a C engine for the MINE suite and its R, 
       Python and MATLAB wrappers. 
       Bioinformatics first published online December 14, 2012
       doi:10.1093/bioinformatics/bts707. 
       http://bioinformatics.oxfordjournals.org/content/early/
       2013/01/06/bioinformatics.bts707
    
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include "mine.h"

char *libmine_version = LIBMINE_VERSION;

#define MAX(a, b) (((a) > (b) ? (a) : (b)))
#define MIN(a, b) (((a) < (b) ? (a) : (b)))

#define TRUE 1
#define FALSE 0


void quicksort(double *a, int *idx, int l, int u)
{
  int i, m, idx_temp;
  double a_temp;


  if (l >= u)
    return;
  
  m = l;
  for (i=l+1; i<=u; i++)
    {
      if (a[i] < a[l])
	{
	  ++m;
	  
	  idx_temp = idx[m];
	  idx[m] = idx[i];
	  idx[i] = idx_temp;
	  
	  a_temp = a[m];
	  a[m] = a[i];
	  a[i] = a_temp;
	}
    }
  
  idx_temp = idx[l];
  idx[l] = idx[m];
  idx[m] = idx_temp;
  
  a_temp = a[l];
  a[l] = a[m];
  a[m] = a_temp;
  
  quicksort(a, idx, l, m-1);
  quicksort(a, idx, m+1, u);
}


int *argsort(double *a, int n)
{
  double *a_cpy;
  int i, *idx;
  
  a_cpy = (double *) malloc(n * sizeof(double));
  if (a_cpy == NULL)
    return NULL;
  
  idx = (int *) malloc(n * sizeof(int));
  if (idx == NULL)
    {
      free(a_cpy);
      return NULL;
    }
   
  /* fill a_cpy */
  memcpy(a_cpy, a, n * sizeof(double));

  /* fill idx */
  for (i=0; i<n; i++)
    idx[i] = i;
  
  quicksort(a_cpy, idx, 0, n-1);
  free(a_cpy);
  
  return idx;
}


/* Returns the entropy induced by the points on the partition Q.
 * See section 3.2.1, page 10, SOM.
 *
 * Parameters
 *   cumhist : cumulative histogram matrix along P_map
 *   q : number of rows of cumhist (number of partitions in Q_map)
 *   p : number of cols of cumhist (number of partitions in P_map)
 *   n : total number of points
 */
double hq(int **cumhist, int q, int p, int n)
{
  int i;
  double prob, H = 0.0;
  
  double total = (double) n;
  
  for (i=0; i<q; i++)
    {
      if (cumhist[i][p-1] != 0)
	{
	  prob = (double) cumhist[i][p-1] / total;
	  H -= prob * log(prob);
	}
    }
  
  return H;
}


/* Returns the entropy induced by the points on the partition 
 * <c_0, c_s, c_t>. See line 5 of Algorithm 2, SOM.
 *
 * Parameters
 *   c : c_1, ..., c_p
 *   s : s in c_s
 *   t : t in c_t
 */
double hp3(int *c, int s, int t)
{
  int sum;
  double prob, H = 0.0;
  

  double total = (double) c[t-1];

  if (s == t)
    return 0.0;
  
  if (c[s-1] != 0)
    {
      prob = (double) c[s-1] / total;
      H -= prob * log(prob);
    }
  
  sum = c[t-1] - c[s-1];
  if (sum != 0)
    {
      prob = (double) sum / total;
      H -= prob * log(prob);
    }
  
  return H;
}


/* Returns the entropy induced by the points on the partition 
 * <c_0, c_s, c_t>, Q. See line 5 of Algorithm 2 in SOM.
 *
 * Parameters
 *   cumhist : cumulative histogram matrix along P_map
 *   c : c_1, ..., c_p
 *   q : number of rows of cumhist (number of partitions in Q_map)
 *   p : number of cols of cumhist (number of partitions in P_map)
 *   s : s in c_s
 *   t : t in c_t
 */
double hp3q(int **cumhist, int *c, int q, int p, int s, int t)
  
{
  int i, sum;
  double prob, H = 0.0;
  
  
  double total = (double) c[t-1];
  
  for (i=0; i<q; i++) 
    {
      if (cumhist[i][s-1] != 0)
	{
	  prob = (double) cumhist[i][s-1] / total;
	  H -= prob * log(prob);
	}
      
      sum = cumhist[i][t-1] - cumhist[i][s-1];
      if (sum != 0)
	{
	  prob = (double) sum / total;
	  H -= prob * log(prob);
	}
    }
  
  return H;
}


/* Returns the entropy induced by the points on the partition <c_s, c_t>
 * and Q. See line 13 of Algorithm 2, SOM.
 *
 * Parameters
 *   cumhist : cumulative histogram matrix along P_map
 *   c : c_1, ..., c_p
 *   q : number of rows of cumhist (number of partitions
 *       in Q_map)
 *   p : number of cols of cumhist (number of partitions
 *       in P_map)
 *   s : s in c_s
 *   t : t in c_t
 */
double hp2q(int **cumhist, int *c, int q, int p, int s, int t)
{
  int i, sum;
  double prob, H = 0.0 ;
  
  
  double total = (double) (c[t-1] - c[s-1]);
  
  if (s == t)
    return 0.0;
  
  for (i=0; i<q; i++) 
    {
      sum = cumhist[i][t-1] - cumhist[i][s-1];
      if (sum != 0)
	{
	  prob = (double) sum / total;
	  H -= prob * log(prob);
	}
    }
  
  return H;
}


/* Returns the map Q: D -> {0, ...,q-1}. 
 * See Algorithm 3 in SOM.
 * 
 * Parameters
 *   dy (IN): y-data sorted in increasing order
 *   n (IN): number of elements of dy
 *   y (IN): an integer greater than 1
 *   Q_map (OUT) : the map Q. Q_map must be a preallocated vector of 
 *                 size n
 *   q (OUT) : number of partitions in Q_map. q can be < y
 * 
 * Returns
 *   0
 */
int EquipartitionYAxis(double *dy, int n, int y, int *Q_map, int *q)
{
  int i, j, s, h, curr;
  double temp1, temp2;
  
  double rowsize = (double) n / (double) y;
  
  i = 0;
  h = 0;
  curr = 0;

  while (i < n)
    {
      s = 1;
      for (j=i+1; j<n; j++)
	{
	  if (dy[i] == dy[j])
	    ++s;
	  else 
	    break;
	}
      
      temp1 = fabs((double) h + (double) s - rowsize);
      temp2 = fabs((double) h - rowsize);
      if ((h != 0) && (temp1 >= temp2))
	{
	  ++curr;
	  h = 0;
	  temp1 = (double) n - (double) i;
	  temp2 = (double) y - (double) curr;
	  rowsize = temp1 / temp2;
	}
      
      for (j=0; j<s; j++)
	Q_map[i+j] = curr;
    
      i += s;
      h += s;
    }
  
  *q = curr + 1;
  
  return 0;
}


/* Returns the map P: D -> {0, ...,p-1}.
 * 
 * Parameters
 *   dx (IN) : x-data sorted in increasing order
 *   n (IN) : number of elements of dx
 *   Q_map (IN) : the map Q computed by EquipartitionYAxis sorted in 
 *                increasing order by dx-values
 *   P_map (OUT) : the map P. P_map must be a preallocated vector
 *                 of size n
 *   p (OUT) : number of partitions in P_map
 *
 * Returns
 *   0 on success, 1 if an error occurs
 */
int GetClumpsPartition(double *dx, int n, int *Q_map, int *P_map, int *p)
{
  int i, j, flag, c, s;
  int *Q_tilde;
  
  i = 0;
  c = -1;
  
  Q_tilde = (int *) malloc (n * sizeof(int));
  if (Q_tilde == NULL)
    return 1;
  
  memcpy(Q_tilde, Q_map, n*sizeof(int));
  
  while (i < n)
    {
      s = 1;
      flag = FALSE;
      for (j=i+1; j<n; j++)
	{
	  if (dx[i] == dx[j])
	    {
	      if (Q_tilde[i] != Q_tilde[j])
		flag = TRUE;
	      ++s;
	    }
	  else
	    break;
	}
      
      if ((s > 1) && (flag == TRUE))
	{
	  for (j=0; j<s; j++)
	    Q_tilde[i+j] = c;
	  --c;
	}
      
      i += s;
    }
  
  i = 0;
  P_map[0] = 0;
  for (j=1; j<n; j++)
    {
      if (Q_tilde[j] != Q_tilde[j-1])
	++i;
      P_map[j] = i;
    }
  
  *p = i + 1;
  free(Q_tilde);
  
  return 0;
}


/* Returns the map P: D -> {0, ...,p-1}.
 * 
 * Parameters
 *   dx (IN) : x-data sorted in increasing order
 *   n (IN) : number of elements of dx
 *   k_hat (IN) : maximum number of clumps
 *   Q_map (IN) : the map Q computed by EquipartitionYAxis sorted in 
 *                increasing order by dx-values
 *   P_map (OUT) : the map P. P_map must be a preallocated vector
 *                 of size n
 *   p (OUT) : number of partitions in P_map
 *
 * Returns
 *   0 on success, 1 if an error occurs
 */
int GetSuperclumpsPartition(double *dx, int n, int k_hat, int *Q_map, 
			    int *P_map, int *p)
{
  int i, ret;
  double *dp;
   
  /* clumps */
  ret = GetClumpsPartition(dx, n, Q_map, P_map, p);
  if (ret)
    return 1;
  
  /* superclumps */
  if (*p > k_hat)
    {
      dp = (double *) malloc (n * sizeof(double));
      if (dp == NULL)
	return 1;
      
      for (i=0; i<n; i++)
	dp[i] = (double) P_map[i];
      
      EquipartitionYAxis(dp, n, k_hat, P_map, p);
      
      free(dp);
    }
  
  return 0;
}


/* Returns (c_1, ..., c_k) */
int *compute_c(int *P_map, int p, int n)
{
  int i;
  int *c;

  
  c = (int *) malloc (p * sizeof(int));
  if (c == NULL)
    return NULL;
  
  for (i=0; i<p; i++)
    c[i] = 0;
  
  for (i=0; i<n; i++)
    c[P_map[i]]++;

  for (i=1; i<p; i++)
    c[i] += c[i-1];
  
  return c;
}


/* Returns the cumulative histogram matrix along P_map */
int **compute_cumhist(int *Q_map, int q, int *P_map, int p, int n)
{
  int i, j;
  int **cumhist;

  
  cumhist = (int **) malloc (q * sizeof(int *));
  if (cumhist == NULL)
    return NULL;
  
  for (i=0; i<q; i++)
    {
      cumhist[i] = (int *) malloc (p * sizeof(int));
      if (cumhist[i] == NULL)
	{
	  for (j=0; j<i; j++)
	    free(cumhist[j]);
	  free(cumhist);
	  return NULL;
	}
      
      for (j=0; j<p; j++)
	cumhist[i][j] = 0;
    }
  
  for (i=0; i<n; i++)
    cumhist[Q_map[i]][P_map[i]]++;

  for (i=0; i<q; i++)
    for (j=1; j<p; j++)
      cumhist[i][j] += cumhist[i][j-1];

  return cumhist;
}


/* Initializes the I matrix */
double **init_I(int p, int x)
{
  int i, j;
  double **I;
  
  
  I = (double **) malloc ((p+1) * sizeof(double *));
  if (I == NULL)
    return NULL;
  
  for (i=0; i<=p; i++)
    {
      I[i] = (double *) malloc ((x+1) * sizeof(double));
      if (I[i] == NULL)
	{
	  for (j=0; j<i; j++)
	    free(I[j]);
	  free(I);
	  return NULL;
	}
      
      for (j=0; j<=x; j++)
	I[i][j] = 0.0;
    }

  return I;
}


/* Computes the HP2Q matrix */
double **compute_HP2Q(int **cumhist, int*c, int q, int p)
{
  int i, j, s, t;
  double **HP2Q;
  
  
  HP2Q = (double **) malloc ((p+1) * sizeof(double *));
  if (HP2Q == NULL)
    return NULL;
  
  for (i=0; i<=p; i++)
    {
      HP2Q[i] = (double *) malloc ((p+1) * sizeof(double));
      if (HP2Q[i] == NULL)
	{
	  for (j=0; j<i; j++)
	    free(HP2Q[j]);
	  free(HP2Q);
	  return NULL;
	}
    }

  for (t=3; t<=p; t++)
    for (s=2; s<=t; s++)
      HP2Q[s][t] = hp2q(cumhist, c, q, p, s, t);

  return HP2Q;
}


/* Returns the normalized MI scores.
 *  
 * Parameters
 *   dx (IN) : x-data sorted in increasing order by dx-values
 *   dy (IN) : y-data sorted in increasing order by dx-values
 *   n (IN) : number of elements in dx and dy
 *   Q_map (IN) : the map Q computed by EquipartitionYAxis() sorted in 
 *                increasing order by dx-values
 *   q (IN) : number of partitions in Q_map
 *   P_map (IN) : the map P computed by GetSuperclumpsPartition() sorted 
 *                in increasing order by Dx-values
 *   p (IN) : number of partitions in P_map
 *   x (IN) : maximum grid size on dx-values
 *   score (OUT) : mutual information scores. score must be a 
 *                 preallocated array of dimension x-1
 * Returns
 *   0 on success, 1 if an error occurs
 */
int OptimizeXAxis(double *dx, double *dy, int n, int *Q_map, int q, 
		  int *P_map, int p, int x, double *score)
{
  int i, s, t, l;
  int *c;
  int **cumhist;
  double **I, **HP2Q;
  double F, F_max, HQ, ct, cs;
  
  
  /* return score=0 if p=1 */
  if (p == 1)
    {
      for (i=0; i<x-1; i++)
	score[i] = 0.0;
      return 0;
    }
 
  /* compute c */
  c = compute_c(P_map, p, n);
  if (c == NULL)
    goto error_c;
  
  /* compute the cumulative histogram matrix along P_map */
  cumhist = compute_cumhist(Q_map, q, P_map, p, n);
  if (cumhist == NULL)
    goto error_cumhist;

  /* I matrix initialization */
  I = init_I(p, x);
  if (I == NULL)
    goto error_I;

  /* Precomputes the HP2Q matrix */
  HP2Q = compute_HP2Q(cumhist, c, q, p);
  if (HP2Q == NULL)
    goto error_HP2Q;

  /* compute H(Q) */
  HQ = hq(cumhist, q, p, n);

  /* 
   * Find the optimal partitions of size 2
   * Algorithm 2 in SOM, lines 3-8
   */
  for (t=2; t<=p; t++)
    {
      F_max = -DBL_MAX;
      for (s=1; s<=t; s++)
	{
	  F = hp3(c, s, t) - hp3q(cumhist, c, q, p, s, t);
	  if (F > F_max)
	    {
	      I[t][2] = HQ + F;
	      F_max = F;
	    }
	}
    }
  
  /* 
   * Inductively build the rest of the table of
   * optimal partitions
   * Algorithm 2 in SOM, lines 10-17
   */
  for (l=3; l<=x; l++)
    {
      for (t=l; t<=p; t++)
	{
	  ct = (double) c[t-1];
	  F_max = -DBL_MAX;
	  for (s=l-1; s<=t; s++)
	    {
	      cs = (double) c[s-1];
	      F = ((cs/ct) * (I[s][l-1]-HQ))
		- (((ct-cs)/ct) * HP2Q[s][t]);
	      
	      if (F > F_max)
		{
		  I[t][l] = HQ + F;
		  F_max = F;
		}
	    }
	}
    }
  
  /* Algorithm 2 in SOM, line 19 */
  for (i=p+1; i<=x; i++)
    I[p][i] = I[p][p];
  
  /* score */
  for (i=2; i<=x; i++)
    score[i-2] = I[p][i] / MIN(log(i), log(q));
 
  /* start frees */
  for (i=0; i<=p; i++)
    free(HP2Q[i]);
  free(HP2Q);

  for (i=0; i<=p; i++)
    free(I[i]);
  free(I);
  
  for (i=0; i<q; i++)
    free(cumhist[i]);
  free(cumhist);
  
  free (c);
  /* end frees*/

  return 0;
  
  /* gotos*/
 error_HP2Q:
  for (i=0; i<=p; i++)
    free(I[i]);
  free(I);
 error_I:
  for (i=0; i<q; i++)
    free(cumhist[i]);
  free(cumhist);
 error_cumhist:
  free(c);
 error_c:
  return 1;
}


/* Returns an initialized mine_score structure.
 * Returns NULL if an error occurs.
 */
mine_score *init_score(mine_problem *prob, mine_parameter *param)
{
  int i, j;
  double B;
  mine_score *score;

  
  B = MAX(pow(prob->n, param->alpha), 4);
  
  score = (mine_score *) malloc (sizeof(mine_score));
  if (score == NULL)
    goto error_score;

  score->n = MAX((int) floor(B/2.0), 2) - 1;
  score->m = (int *) malloc(score->n * sizeof(int));
  if (score->m == NULL)
    goto error_score_m;
  
  for (i=0; i<score->n; i++)
    score->m[i] = (int) floor((double) B / (double) (i+2)) - 1;
  
  score->M = (double **) malloc (score->n * sizeof(double *));
  if (score->M == NULL)
    goto error_score_M;
   
  for (i=0; i<score->n; i++)
    {
      score->M[i] = (double *) malloc ((score->m[i]) * sizeof(double));
      if (score->M[i] == NULL)
	{
	  for (j=0; j<i; j++)
	    free(score->M[j]);
	  goto error_score_M_i;
	}
    }
  
  return score;
 
 error_score_M_i:
  free(score->M);
 error_score_M:
  free(score->m);
 error_score_m:
  free(score);
 error_score:
  return NULL;
}

/* Computes the maximum normalized mutual information scores and 
 * returns a mine_score structure. Returns NULL if an error occurs.
 * Algorithm 5, page 14, SOM
 */
mine_score *mine_compute_score(mine_problem *prob, mine_parameter *param)
{
  int i, j, k, p, q, ret;
  double *xx, *yy, *xy, *yx, *M_temp;
  int *ix, *iy;
  int *Q_map_temp, *Q_map, *P_map;
  mine_score *score;

  score = init_score(prob, param);
  if (score == NULL)
    goto error_score;

  xx = (double *) malloc (prob->n * sizeof(double));
  if (xx == NULL)
    goto error_xx;
  
  yy = (double *) malloc (prob->n * sizeof(double));
  if (yy == NULL)
    goto error_yy;
  
  xy = (double *) malloc (prob->n * sizeof(double));
  if (xy == NULL)
    goto error_xy;
  
  yx = (double *) malloc (prob->n * sizeof(double));
  if (yx == NULL)
    goto error_yx;

  Q_map_temp = (int *) malloc (prob->n * sizeof(int));
  if (Q_map_temp == NULL)
    goto error_Q_temp;
  
  Q_map = (int *) malloc (prob->n * sizeof(int));
  if (Q_map == NULL)
    goto error_Q;
  
  P_map = (int *) malloc (prob->n * sizeof(int));
  if (P_map == NULL)
    goto error_P;
  
  ix = argsort(prob->x, prob->n);
  if (ix == NULL)
    goto error_ix;
  
  iy = argsort(prob->y, prob->n);
  if (iy == NULL)
    goto error_iy;
  
  M_temp = (double *)malloc ((score->m[0]) * sizeof(double));
  if (M_temp == NULL)
    goto error_M_temp;

  /* build xx, yy, xy, yx */
  for (i=0; i<prob->n; i++)
    {
      xx[i] = prob->x[ix[i]];
      yy[i] = prob->y[iy[i]];
      xy[i] = prob->x[iy[i]];
      yx[i] = prob->y[ix[i]];
    }
  
  /* x vs. y */
  for (i=0; i<score->n; i++)
    {
      k = MAX((int) (param->c * (score->m[i]+1)), 1);
      
      ret = EquipartitionYAxis(yy, prob->n, i+2, Q_map, &q);
      if (ret)
  	goto error_0;

      /* sort Q by x */
      for (j=0; j<prob->n; j++)
  	Q_map_temp[iy[j]] = Q_map[j];
      for (j=0; j<prob->n; j++)
  	Q_map[j] = Q_map_temp[ix[j]];
      
      ret = GetSuperclumpsPartition(xx, prob->n, k, Q_map,
  				    P_map, &p);
      if (ret)
  	goto error_0;
      
      if (param->est == EST_MIC_APPROX)
  	ret = OptimizeXAxis(xx, yx, prob->n, Q_map, q, P_map, p,
  			    score->m[i]+1, score->M[i]);
      else /* EST_MIC_E */
  	ret = OptimizeXAxis(xx, yx, prob->n, Q_map, q, P_map, p,
  			    MIN(i+2, score->m[i]+1), score->M[i]);

      //printf("i: %i, x: %i , max_y_approx: %i, max_y_e: %i\n", i, i+2, score->m[i]+1, MIN(i+2, score->m[i]+1));

      if (ret)
  	goto error_0;
    }
  
  /* y vs. x */
  for (i=0; i<score->n; i++)
    {
      k = MAX((int) (param->c * (score->m[i]+1)), 1);
      
      ret = EquipartitionYAxis(xx, prob->n, i+2, Q_map, &q);
      if (ret)
  	goto error_0;

      /* sort Q by y */
      for (j=0; j<prob->n; j++)
  	Q_map_temp[ix[j]] = Q_map[j];
      for (j=0; j<prob->n; j++)
  	Q_map[j] = Q_map_temp[iy[j]];
      
      ret = GetSuperclumpsPartition(yy, prob->n, k, Q_map,
  				    P_map, &p);
      if (ret)
  	goto error_0;
      
      if (param->est == EST_MIC_APPROX)
  	ret = OptimizeXAxis(yy, xy, prob->n, Q_map, q, P_map, p,
  			    score->m[i]+1, M_temp);
      else /* EST_MIC_E */
  	ret = OptimizeXAxis(yy, xy, prob->n, Q_map, q, P_map, p,
  			    MIN(i+2, score->m[i]+1), M_temp);

      //printf("i: %i, y: %i , max_x_approx: %i, max_x_e: %i\n", i, i+2, score->m[i]+1, MIN(i+2, score->m[i]+1));

      if (ret)
  	goto error_0;
      
      if (param->est == EST_MIC_APPROX)
  	for (j=0; j<score->m[i]; j++)
  	  score->M[j][i] = MAX(M_temp[j], score->M[j][i]);
      else /* EST_MIC_E */
  	for (j=0; j<MIN(i+1, score->m[i]); j++)
  	  score->M[j][i] = M_temp[j];
   }
  
  free(M_temp);
  free(iy);
  free(ix);
  free(P_map);
  free(Q_map);
  free(Q_map_temp);
  free(yx);
  free(xy);
  free(yy);
  free(xx);

  return score;
    
 error_0:
  free(M_temp);
 error_M_temp: 
  free(iy);
 error_iy:
  free(ix);
 error_ix:
  free(P_map);
 error_P:
  free(Q_map);
 error_Q:
  free(Q_map_temp);
 error_Q_temp:
  free(yx);
 error_yx:
  free(xy);
 error_xy:
  free(yy);
 error_yy:
  free(xx);
 error_xx:
  for (i=0; i<score->n; i++)
    free(score->M[i]);
  free(score->M);
  free(score->m);
  free(score);
 error_score:
  return NULL;
}


/* This function checks the parameters. It should be called 
 * before calling mine_compute_score(). It returns NULL if 
 * the parameters are feasible, otherwise an error message
 * is returned.
 */
char *mine_check_parameter(mine_parameter *param)
{
  if ((param->alpha <= 0.0) || (param->alpha > 1.0))
    return "alpha must be in (0, 1.0]";
  
  if (param->c <= 0.0)
    return "c must be > 0.0";
  
  return NULL;
}


/* Returns the Maximal Information Coefficient (MIC). */
double mine_mic(mine_score *score)
{
  int i, j;
  double score_max = 0.0;
  
  for (i=0; i<score->n; i++)
    for (j=0; j<score->m[i]; j++)
      if (score->M[i][j] > score_max)
	score_max = score->M[i][j];
  
  return score_max;
}


/* Returns the Maximum Asymmetry Score (MAS). */
double mine_mas(mine_score *score)
{
  int i, j;
  double score_curr;
  double score_max = 0.0;
  
  for (i=0; i<score->n; i++)
    for (j=0; j<score->m[i]; j++)
      {
	score_curr = fabs(score->M[i][j] - score->M[j][i]);
	if (score_curr > score_max)
	  score_max = score_curr;
      }
  
  return score_max;
}


/* Returns the Maximum Edge Value (MEV). */
double mine_mev(mine_score *score)
{
  int i, j;
  double score_max = 0.0;
  
  for (i=0; i<score->n; i++)
    for (j=0; j<score->m[i]; j++)
      if (((j==0) || (i==0)) && score->M[i][j] > score_max)
	score_max = score->M[i][j];
  
  return score_max;
}


/* Returns the Minimum Cell Number (MCN) with eps >= 0. */
double mine_mcn(mine_score *score, double eps)
{
  int i, j;
  double log_xy;
  double score_min = DBL_MAX;
  double delta = 0.0001; /* avoids overestimation of mcn */
  double mic = mine_mic(score);

  for (i=0; i<score->n; i++)
    for (j=0; j<score->m[i]; j++)
      {
	log_xy = log((i+2) * (j+2)) / log(2.0);
	if (((score->M[i][j]+delta) >= ((1.0 - eps) * mic)) 
	    && (log_xy < score_min))
	  score_min = log_xy;
      }

  return score_min;
}


/* Returns the Minimum Cell Number (MCN) with eps = 1 - MIC. */
double mine_mcn_general(mine_score *score)
{
  int i, j;
  double log_xy;
  double score_min = DBL_MAX;
  double delta = 0.0001; /* avoids overestimation of mcn */
  double mic = mine_mic(score);
  
  for (i=0; i<score->n; i++)
    for (j=0; j<score->m[i]; j++)
      {
	log_xy = log((i+2) * (j+2)) / log(2.0);
	if (((score->M[i][j]+delta) >= (mic * mic)) 
	    && (log_xy < score_min))
	  score_min = log_xy;
      }

  return score_min;
}


/* Returns the e Generalized Mean Information Coefficient (GMIC) */
double mine_gmic(mine_score *score, double p)
{
  int i, j, k, Z, B;
  mine_score *score_sub, *C_star;
  double gmic;
  
  /* alloc score_sub */
  score_sub = (mine_score *) malloc (sizeof(mine_score));
  
  /* alloc C_star */
  C_star = (mine_score *) malloc (sizeof(mine_score));
  C_star->m = (int *) malloc(score->n * sizeof(int));
  C_star->M = (double **) malloc (score->n * sizeof(double *));
  for (i=0; i<score->n; i++)
    C_star->M[i] = (double *) malloc ((score->m[i]) * sizeof(double));  
     
  /* prepare score_sub */
  score_sub->M = score->M;

  /* prepare C_star */
  C_star->n = score->n;
  for (i=0; i<C_star->n; i++)
    C_star->m[i] = score->m[i];
   
  /* compute C_star */
  for (i=0; i<score->n; i++)
    for (j=0; j<score->m[i]; j++)
      {
	B = (i+2) * (j+2);
	score_sub->n = MAX((int) floor(B/2.0), 2) - 1;
	score_sub->m = (int *) malloc(score_sub->n * sizeof(int));
	for (k=0; k<score_sub->n; k++)
	  score_sub->m[k] = (int) floor((double) B / (double) (k+2)) - 1;
	
	C_star->M[i][j] = mine_mic(score_sub);
	free(score_sub->m);
      }
  
  /* p=0 -> geometric mean */
  if (p == 0.0)
    {
      Z = 0;
      gmic = 1.0;
      for (i=0; i<C_star->n; i++)
	for (j=0; j<C_star->m[i]; j++)
	  {
	    gmic *= C_star->M[i][j];
	    Z++;
	  }
      gmic = pow(gmic, (double) Z);
    }
  /* p!=0 -> generalized mean */
  else
    {
      Z = 0;
      gmic = 0.0;
      for (i=0; i<C_star->n; i++)
	for (j=0; j<C_star->m[i]; j++)
	  {
	    gmic += pow(C_star->M[i][j], p);
	    Z++;
	  }
      gmic /= (double) Z;
      gmic = pow(gmic, 1.0/p);
    }

  free(score_sub);
  if (C_star->n != 0)
    {
      free(C_star->m);
      for (i=0; i<C_star->n; i++)
 	free(C_star->M[i]);
      free(C_star->M);
    }
  free(C_star);

  return gmic;
}


/* Returns the Total Information Coefficient (TIC). */
double mine_tic(mine_score *score)
{
  int i, j;
  double tic = 0.0;
  
  for (i=0; i<score->n; i++)
    for (j=0; j<score->m[i]; j++)
      tic += score->M[i][j];
       
  return tic;
}


/* This function frees the memory used by the 
 * mine_score structure.
 */
void mine_free_score(mine_score **score)
{
  int i;
  mine_score *score_ptr = *score;

  if (score_ptr != NULL)
    {
      if (score_ptr->n != 0)
	{
	  free(score_ptr->m);
	  for (i=0; i<score_ptr->n; i++)
	    free(score_ptr->M[i]);
	  free(score_ptr->M);
	}
      
      free(score_ptr);
      score_ptr = NULL;
    }
}
