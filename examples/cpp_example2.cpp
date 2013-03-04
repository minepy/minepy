#include <cstdlib>
#include <cmath>
#include <iostream>
#include "cppmine.h"

using namespace std;

int
main (int argc, char **argv)
{
  int n = 7;
  double x[] = {1.,2.,3.,4.,5.,6.,7.};
  double y[] = {1.,2.,3.,4.,3.,2.,1.};
  
  /* build the MINE object */
  MINE mine(0.6, 15);

  /* compute score */
  mine.compute_score(x, y, n);
  
  /* print MIC */
  cout << "MIC: " << mine.mic() << "\n";
    
  return 0;
}
