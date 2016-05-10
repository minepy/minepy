#ifndef _LIBMINE_H
#define _LIBMINE_H

#define LIBMINE_VERSION "1.1.0"

#define EST_MIC_APPROX 0 /* The original MIC estimator */
#define EST_MIC_E      1 /* MIC_e (TIC_e) estimator */

#ifdef __cplusplus
extern "C" {
#endif

extern char *libmine_version;

/*
 * The mine_problem structure describes the problem. x and y are the two
 * variables of length n.
 */
typedef struct mine_problem
{
  int n;
  double *x;
  double *y;
} mine_problem;

/*
 * The mine_parameter structure describes the MINE parameters. Alpha is the
 * exponent in B(n) = n^alpha and must be in (0,1] and c determines how many
 * more clumps there will be than columns in every partition. c = 15 meaning
 * that when trying to draw x grid lines on the x-axis, the algorithm will
 * start with at most 15*x clumps. c must be > 0. Est is the estimator,
 * EST_MIC_APPROX and EST_MIC_E. With est=EST_MIC_APPROX, the mine_*()
 * functions return the original statistics, with est=EST_MIC_E the
 * equicharacteristic matrix is computed and mine_mic() and mine_tic() will
 * return MIC_e and TIC_e respectively.
 */
typedef struct mine_parameter
{
  double alpha;
  double c;
  int est;
} mine_parameter;

/*
 * The mine_score structure describes the maximum normalized mutual information
 * scores (i.e. the characteristic matrix if est=EST_MIC_APPROX, the
 * equicharacteristic matrix instead). M[i][j] contains the score using a grid
 * partitioning x-values into i+2 bins and y-values into j+2 bins. m and M are
 * of length n and each M[i] is of length m[i].
 */
typedef struct mine_score
{
  int n;      /* number of rows of M */
  int *m;     /* number of cols of M[i] for each i */
  double **M; /* the (equi)characteristic matrix */
} mine_score;

/*
 * Computes the (equi)characteristic matrix (i.e. maximum normalized mutual
 * information scores and returns a mine_score structure. Returns NULL if an
 * error occurs.
 */
mine_score *mine_compute_score(mine_problem *prob, mine_parameter *param);

/*
 * Checks the parameters. This function should be called before calling
 * mine_compute_score(). It returns NULL if the parameters are feasible,
 * otherwise an error message is returned.
 */
char *mine_check_parameter(mine_parameter *param);

/* Returns the Maximal Information Coefficient (MIC or MIC_e).  */
double mine_mic(mine_score *score);

/* Returns the Maximum Asymmetry Score (MAS). */
double mine_mas(mine_score *score);

/* Returns the Maximum Edge Value (MEV). */
double mine_mev(mine_score *score);

/* Returns the Minimum Cell Number (MCN), with eps >= 0. */
double mine_mcn(mine_score *score, double eps);

/* Returns the Minimum Cell Number (MCN) with eps = 1 - MIC. */
double mine_mcn_general(mine_score *score);

/* Returns the Generalized Mean Information Coefficient (GMIC). */
double mine_gmic(mine_score *score, double p);

/* Returns the Total Information Coefficient (TIC and TIC_e). */
double mine_tic(mine_score *score);

/*
 * This function frees the memory used by a mine_score and destroys the score
 * structure.
 */
void mine_free_score(mine_score **score);

#ifdef __cplusplus
}
#endif

#endif /* _LIBMINE_H */
