#include <cstdlib>
#include <cmath>
#include <iostream>
#include "cppmine.h"

using namespace std;

int
main (int argc, char **argv)
{
  double PI;
  int i, n;
  double *x, *y;
  MINE *mine;

  PI = 3.14159265;

  /* build the MINE object with exceptions management */
  try
    {
      mine = new MINE(0.6, 15, EST_MIC_APPROX);
    }
  catch (char *s)
    {
      cout << "WARNING: " << s << "\n";
      cout << "MINE will be set with alpha=0.6 and c=15" << "\n";
      mine = new MINE(0.6, 15, EST_MIC_APPROX);
    }

  /* build the problem */
  n = 1001;
  x = new double [n];
  y = new double [n];
  for (i=0; i<n; i++)
    {
      /* build x = [0, 0.001, ..., 1] */
      x[i] = (double) i / (double) (n-1);

      /* build y = sin(10 * pi * x) + x */
      y[i] = sin(10 * PI * x[i]) + x[i];
    }

  /* compute score with exceptions management */
  try
    {
      mine->compute_score(x, y, n);
    }
  catch (char *s)
    {
      cout << "ERROR: " << s << "\n";
      return 1;
    }

  /* print mine statistics */
  try
    {
      cout << "MIC: " << mine->mic() << "\n";
      cout << "MAS: " << mine->mas() << "\n";
      cout << "MEV: " << mine->mev() << "\n";
      cout << "MCN (eps=0): " << mine->mcn(0) << "\n";
      cout << "MCN (eps=1-MIC): " << mine->mcn_general() << "\n";
      cout << "TIC: " << mine->tic() << "\n";
    }
  catch (char *s)
    {
      cout << "ERROR: " << s << "\n";
      return 1;
    }

  /* delete the mine object */
  delete mine;

  /* free the problem */
  delete [] x;
  delete [] y;

  return 0;
}
