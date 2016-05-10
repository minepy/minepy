#ifndef _CPPMINE_H
#define _CPPMINE_H

#include "mine.h"

class MINE {

  private:

    mine_parameter param;
    mine_problem prob;
    mine_score *score;

  public:

    /* Constructor */
    MINE(double alpha, double c, int est);

    /* Destructor. */
    ~MINE();

    /* See mine.h */
    void compute_score(double *x, double *y, int n);

    /* See mine.h */
    double mic();

    /* See mine.h */
    double mas();

    /* See mine.h */
    double mev();

    /* See mine.h */
    double mcn(double eps);

    /* See mine.h */
    double mcn_general();

    /* See mine.h */
    double gmic(double p);

    /* See mine.h */
    double tic();
};

#endif /* _CPPMINE_H */
