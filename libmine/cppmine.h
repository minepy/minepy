#ifndef _CPPMINE_H
#define _CPPMINE_H

#include "mine.h"

class MINE {
  
 private:

  mine_parameter param; /* defined in "mine.h" */
  mine_problem prob; /* defined in "mine.h" */
  mine_score *score; /* defined in "mine.h" */
  
 public:
  
  /* Constructor */
  MINE(double alpha, double c);

  /* Destructor. */
  ~MINE();

  /* Computes the maximum normalized mutual information scores
   * the between variables x and y of length n.
   */
  void compute_score(double *x, double *y, int n);
  
  /* Returns the Maximal Information Coefficient (MIC). */
  double mic();
  
  /* Returns the Maximum Asymmetry Score (MAS). */
  double mas();
  
  /* Returns the Maximum Edge Value (MEV). */
  double mev();
  
  /* Returns the Minimum Cell Number (MCN), with eps >= 0. */
  double mcn(double eps);

  /* Returns the Minimum Cell Number (MCN) with eps = 1 - MIC. */
  double mcn_general();

  /* Returns the e Generalized Mean Information Coefficient (GMIC) */
  double gmic(double p);

  /* Returns the Total Information Coefficient (TIC). */
  double tic();
};

#endif /* _CPPMINE_H */
