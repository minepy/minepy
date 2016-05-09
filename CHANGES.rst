CHANGES
=======

1.1.0
-----
* libmine, C++, and Python API: MIC_e and TIC estimators added
* libmine, C++, and Python API: Generalized Mean Information Coefficient (GMIC) added
* Python API: nogil added to computationally intensive parts (thanks to Adrin
  Jalali - adrinjalali)
* Indentations adjusted

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
