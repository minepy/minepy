CHANGES
=======

1.2.3
-----
* Fix Python 3.7 issue.

1.2.2
-----
* mine command-line application removed. Use MICtools 
(https://github.com/minepy/mictools) instead.

1.2.1
-----
* Now libmine library precomputes log(cumhist) and log(c):
  10-15% faster than the previous version.
* 'pip install minepy' now install the numpy dependency without errors

1.2.0
-----
* Now if alpha is >=4 then alpha defines directly the B parameter
* Convenience functions (pstats and cstats) for C and Python API added
* Documentation updated
* Normalization option for TIC added to C, C+= and Python APIs

1.2.0
-----
* Now if alpha is >=4 then alpha defines directly the B parameter
* Convenience functions (pstats and cstats) for C and Python API added
* Documentation updated
* Normalization option for TIC added to C, C+= and Python APIs

1.1.0
-----
* minestats() and pearson() functions removed from the Python module
* get_alpha() and get_c() methods removed from the Python MINE class
* Matlab API: MIC_e estimator added (thanks to Robin Ince - robince)
* Matlab API: TIC statistic added (thanks to Robin Ince - robince)
* libmine (C), C++, and Python API: MIC_e and TIC estimators added
* libmine (C), C++, and Python API: TIC statistic added
* libmine (C), C++, and Python API: Generalized Mean Information Coefficient
  (GMIC) added
* Python API: nogil added to computationally intensive parts (thanks to Adrin
  Jalali - adrinjalali)
* Documentation improved

1.0.0
-----
* libmine: core.h included in mine.h
* libmine: speed improvements
* libmine: char *libmine_version added
* libmine: struct mine_score changed
* libmine: mic(), mas(), mev(), mcn(), check_parameter() was renamed
  to mine_mic(), mine_mas(), mine_mev(), mine_mcn() and mine_check_parameter()
* libmine: mine_mcn(): parameter eps added
* libmine: mine_mcn_general() added
* C++: get_mic(), get_mas(), get_mev() and get_mcn() methods was renamed
  to mic(), mas(), mev() and mcn()
* C++: mcn(): parameter eps added
* C++: mcn_general() method added
* Python: score() method was renamed to compute_score()
* Python: mcn(): parameter eps added
* Python: mcn_general() method added
* Python: convenience functions minestats() and pearson added
* Matlab: mcn_general was added in output
* Mine Application: mcn_general was added in output
* Documentation improved

0.3.5
-----
* documentation improved
* mine application: extra space in header removed
* C++ interface (libmine folder, MINE class)

0.3.4
-----
* MATLAB/OCTAVE interface added ("matlab" folder)
* code documentation improved
* API and command line documentation improved
* mine script optimized avoiding indexes preallocation
* "examples" folder added
* computation of maximum number of clumps fixed

0.3.3
-----
* memory leak fixed

0.3.2
-----
* internal sort function (heapsort) replaced
  with quicksort

0.3.1
-----
* Fix mcn() when x or y = 2
