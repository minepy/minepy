## This code is written by Davide Albanese, <albanese@fbk.eu>
## Copyright (C) 2012 Fondazione Bruno Kessler
## Copyright (C) 2012 Davide Albanese

## This program is free software: you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.

## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.

## You should have received a copy of the GNU General Public License
## along with this program.  If not, see <http://www.gnu.org/licenses/>.


import numpy as np
cimport numpy as np
from libc.stdlib cimport *
cimport cython

# import structures and functions from mine.h
cdef extern from "../libmine/mine.h":

    ctypedef struct mine_problem:
         int n
         double *x
         double *y
         
    ctypedef struct mine_parameter:
         double alpha
         double c
         int est

    ctypedef struct mine_score:
         int n
         int *m
         double **M

    char *libmine_version
         
    mine_score *mine_compute_score (mine_problem *prob, mine_parameter *param) nogil
    char *mine_check_parameter(mine_parameter *param) nogil
    double mine_mic (mine_score *score) nogil
    double mine_mas (mine_score *score) nogil
    double mine_mev (mine_score *score) nogil
    double mine_mcn (mine_score *score, double eps) nogil
    double mine_mcn_general (mine_score *score) nogil
    double mine_gmic (mine_score *score, double p) nogil
    double mine_tic (mine_score *score) nogil
    void mine_free_score (mine_score **score) nogil

    int EST_MIC_APPROX
    int EST_MIC_E

    
version = libmine_version


EST = {
	"mic_approx": EST_MIC_APPROX,
	"mic_e": EST_MIC_E
	}

cdef class MINE:
    """Maximal Information-based Nonparametric Exploration.         
    """

    cdef mine_problem prob
    cdef mine_parameter param
    cdef mine_score *score
    
    def __cinit__(self, alpha=0.6, c=15, est="mic_approx"):
        """
        :Parameters:
            alpha : float (0, 1.0]
                the exponent in B(n) = n^alpha
            c : float (> 0)
                determines how many more clumps there will be than
                columns in every partition. Default value is 15, meaning
                that when trying to draw x grid lines on the x-axis,
                the algorithm will start with at most 15*x clumps
            est : str ("mic_approx", "mic_e")
                specifies the estimator used to obtain the MIC values
                "mic_approx" is the original MIC estimate defined in Reshef
                et al. (2011)
                "mic_e" is the estimator presented in Reshef et al. (2015)
                arXiv:1505.02213v2)

        """
        
        self.param.c = <double> c
        self.param.alpha = <double> alpha
        self.param.est = <int> EST[est]
        self.score = NULL

        ret = mine_check_parameter(&self.param)
        if ret:
            raise ValueError(ret)

    def compute_score(self, x, y):
        """Computes the maximum normalized mutual information scores
        between `x` and `y`.
        """
        cdef int i, j
        cdef np.ndarray[np.float_t, ndim=1] xa, ya

        xa = np.ascontiguousarray(x, dtype=np.float)
        ya = np.ascontiguousarray(y, dtype=np.float)

        if xa.shape[0] != ya.shape[0]:
            raise ValueError("x, y: shape mismatch")

        self.prob.n = <int> xa.shape[0]
        self.prob.x = <double *> xa.data
        self.prob.y = <double *> ya.data

        with nogil:

            self._free_score()
            self.score = mine_compute_score(&self.prob, &self.param)

        if self.score is NULL:
            raise ValueError("problem with mine_compute_score()")

    cdef void _free_score(self) nogil:
        mine_free_score(&self.score)

    def __dealloc__(self):
        self._free_score()

    def mic(self):
        """Returns the Maximal Information Coefficient (MIC).
        """

        if self.score is NULL:
            raise ValueError("no score computed")
        
        return mine_mic(self.score)

    def mas(self):
        """Returns the Maximum Asymmetry Score (MAS).
        """

        if self.score is NULL:
            raise ValueError("no score computed")
        
        return mine_mas(self.score)

    def mev(self):
        """Returns the Maximum Edge Value (MEV).
        """

        if self.score is NULL:
            raise ValueError("no score computed")
        
        return mine_mev(self.score)

    def mcn(self, eps=0):
        """Returns the Minimum Cell Number (MCN) with eps >= 0.
        """

        if self.score is NULL:
            raise ValueError("no score computed")
        
        return mine_mcn(self.score, eps)

    def mcn_general(self):
        """Returns the Minimum Cell Number (MCN) with eps = 1 - MIC.
        """

        if self.score is NULL:
            raise ValueError("no score computed")
        
        return mine_mcn_general(self.score)

    def gmic(self, p=-1):
        """Returns the Generalized Maximal Information Coefficient (GMIC)
        """

        if self.score is NULL:
            raise ValueError("no score computed")
        
        return mine_gmic(self.score, p)

    def tic(self):
        """Returns the Total Information Coefficient (TIC)
        """

        if self.score is NULL:
            raise ValueError("no score computed")
        
        return mine_tic(self.score)
    
    @cython.boundscheck(True)
    def get_score(self):
        """Returns the maximum normalized mutual information scores, M. 
        
        M is a list of 1d numpy arrays where M[i][j] contains the score
        using a grid partitioning x-values into i+2 bins and y-values 
        into j+2 bins.      
        """
        
        cdef int i, j

        if self.score is NULL:
            raise ValueError("no score computed")

        M = []
        for i in range(self.score.n):
            M_temp = np.empty(self.score.m[i], dtype=np.float)
            for j in range(self.score.m[i]):
                M_temp[j] = self.score.M[i][j]
            M.append(M_temp)
    
        return M

    def computed(self):
        """Return True if the score is computed."""
        
        if self.score is NULL:
            return False
        else:
            return True
        
    def get_alpha( self):
        """Returns alpha."""
        
        return self.param.alpha
    
    def get_c(self):
        """Returns c."""
        
        return self.param.c
