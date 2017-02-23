## minepy python module

## This code is written by Davide Albanese, <davide.albanese@gmail.com>
## Copyright (C) 2012-2017 Davide Albanese
## Copyright (C) 2012 Fondazione Bruno Kessler

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


from __future__ import division

import numpy as np
cimport numpy as np
from libc.stdlib cimport *
from minedecl cimport *
cimport cython

np.import_array()

version = libmine_version


EST = {
    "mic_approx": EST_MIC_APPROX,
	"mic_e": EST_MIC_E
	}

cdef class MINE:
    """Maximal Information-based Nonparametric Exploration.

    Parameters
    ----------
    alpha : float (0, 1.0] or >=4
        if alpha is in (0,1] then B will be max(n^alpha, 4) where n is the
        number of samples. If alpha is >=4 then alpha defines directly the B
        parameter. If alpha is higher than the number of samples (n) it will
        be limited to be n, so B = min(alpha, n).
    c : float (> 0)
        determines how many more clumps there will be than columns in
        every partition. Default value is 15, meaning that when trying to
        draw x grid lines on the x-axis, the algorithm will start with at
        most 15*x clumps.
    est : str ("mic_approx", "mic_e")
        estimator. With est="mic_approx" the original MINE statistics will
        be computed, with est="mic_e" the equicharacteristic matrix is
        is evaluated and the mic() and tic() methods will return MIC_e and
        TIC_e values respectively.
    """

    cdef mine_problem prob
    cdef mine_parameter param
    cdef mine_score *score

    def __cinit__(self, alpha=0.6, c=15, est="mic_approx"):
        self.param.c = <double> c
        self.param.alpha = <double> alpha
        self.param.est = <int> EST[est]
        self.score = NULL

        ret = mine_check_parameter(&self.param)
        if ret:
            raise ValueError(ret)

    def compute_score(self, x, y):
        """Computes the (equi)characteristic matrix (i.e. maximum normalized
        mutual information scores.
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
        """Returns the Maximal Information Coefficient (MIC or MIC_e).
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
        """Returns the Generalized Maximal Information Coefficient (GMIC).
        """

        if self.score is NULL:
            raise ValueError("no score computed")

        return mine_gmic(self.score, p)

    def tic(self, norm=False):
        """Returns the Total Information Coefficient (TIC or TIC_e). If
        norm==True TIC will be normalized in [0, 1].
        """

        if self.score is NULL:
            raise ValueError("no score computed")

        return mine_tic(self.score, norm)

    @cython.boundscheck(True)
    def get_score(self):
        """Returns the maximum normalized mutual information scores (i.e. the
        characteristic matrix M if est="mic_approx", the equicharacteristic
        matrix instead). M is a list of 1d numpy arrays where M[i][j] contains
        the score using a grid partitioning x-values into i+2 bins and y-values
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
        """Return True if the (equi)characteristic matrix) is computed.
        """

        if self.score is NULL:
            return False
        else:
            return True


@cython.boundscheck(False)
def pstats(X, alpha=0.6, c=15, est="mic_approx"):
    """Compute pairwise statistics (MIC and normalized TIC) between variables
    (convenience function).

    For each statistic, the upper triangle of the matrix is stored by row
    (condensed matrix). If m is the number of variables, then for i < j < m, the
    statistic between (row) i and j is stored in k = m*i - i*(i+1)/2 - i - 1 + j.
    The length of the vectors is n = m*(m-1)/2.

    Parameters
    ----------
    X : 2D array_like object
        An n-by-m array of n variables and m samples.
    alpha : float (0, 1.0] or >=4
        if alpha is in (0,1] then B will be max(n^alpha, 4) where n is the
        number of samples. If alpha is >=4 then alpha defines directly the B
        parameter. If alpha is higher than the number of samples (n) it will be
        limited to be n, so B = min(alpha, n).
    c : float (> 0)
        determines how many more clumps there will be than columns in
        every partition. Default value is 15, meaning that when trying to
        draw x grid lines on the x-axis, the algorithm will start with at
        most 15*x clumps.
    est : str ("mic_approx", "mic_e")
        estimator. With est="mic_approx" the original MINE statistics will
        be computed, with est="mic_e" the equicharacteristic matrix is
        is evaluated and MIC_e and TIC_e are returned.

    Returns
    -------
    mic : 1D ndarray
        the condensed MIC statistic matrix of length n*(n-1)/2.
    tic : 1D ndarray
        the condensed normalized TIC statistic matrix of length n*(n-1)/2.
    """

    cdef int n, d
    cdef mine_parameter param
    cdef mine_matrix Xm
    cdef mine_pstats *pstats
    cdef np.ndarray[np.float_t, ndim=2] Xa
    cdef np.ndarray[np.float_t, ndim=1] mica, tica
    cdef np.npy_intp shape[1]


    param.c = <double> c
    param.alpha = <double> alpha
    param.est = <int> EST[est]

    ret = mine_check_parameter(&param)
    if ret:
        raise ValueError(ret)

    Xa = np.ascontiguousarray(X, dtype=np.float)
    Xm.data = <double *> Xa.data
    Xm.n = <int> Xa.shape[0]
    Xm.m = <int> Xa.shape[1]

    pstats = mine_compute_pstats(&Xm, &param)

    shape[0] = <np.npy_intp> pstats.n
    mica = np.PyArray_SimpleNewFromData(1, shape, np.NPY_DOUBLE,
                                        <void *> pstats.mic)
    np.PyArray_UpdateFlags(mica, mica.flags.num | np.NPY_OWNDATA)

    tica = np.PyArray_SimpleNewFromData(1, shape, np.NPY_DOUBLE,
                                        <void *> pstats.tic)
    np.PyArray_UpdateFlags(tica, tica.flags.num | np.NPY_OWNDATA)

    free(pstats)

    return mica, tica


@cython.boundscheck(False)
def cstats(X, Y, alpha=0.6, c=15, est="mic_approx"):
    """Compute statistics (MIC and normalized TIC) between each pair of the two
    collections of variables (convenience function).

    If n and m are the number of variables in X and Y respectively, then the
    statistic between the (row) i (for X) and j (for Y) is stored in mic[i, j]
    and tic[i, j].

    Parameters
    ----------
    X : 2D array_like object
        An n by m array of n variables and m samples.
    Y : 2D array_like object
        An p by m array of p variables and m samples.
    alpha : float (0, 1.0] or >=4
        if alpha is in (0,1] then B will be max(n^alpha, 4) where n is the
        number of samples. If alpha is >=4 then alpha defines directly the B
        parameter. If alpha is higher than the number of samples (n) it will be
        limited to be n, so B = min(alpha, n).
    c : float (> 0)
        determines how many more clumps there will be than columns in
        every partition. Default value is 15, meaning that when trying to
        draw x grid lines on the x-axis, the algorithm will start with at
        most 15*x clumps.
    est : str ("mic_approx", "mic_e")
        estimator. With est="mic_approx" the original MINE statistics will
        be computed, with est="mic_e" the equicharacteristic matrix is
        is evaluated and MIC_e and TIC_e are returned.

    Returns
    -------
    mic : 2D ndarray
        the MIC statistic matrix (n x p).
    tic : 2D ndarray
        the normalized TIC statistic matrix (n x p).
    """

    cdef int n, d
    cdef mine_parameter param
    cdef mine_matrix Xm, Ym
    cdef mine_cstats *cstats
    cdef np.ndarray[np.float_t, ndim=2] Xa, Ya
    cdef np.ndarray[np.float_t, ndim=2] mica, tica
    cdef np.npy_intp shape[2]


    param.c = <double> c
    param.alpha = <double> alpha
    param.est = <int> EST[est]

    ret = mine_check_parameter(&param)
    if ret:
        raise ValueError(ret)

    Xa = np.ascontiguousarray(X, dtype=np.float)
    Ya = np.ascontiguousarray(Y, dtype=np.float)

    if X.shape[1] != Y.shape[1]:
        raise ValueError("X, Y: shape mismatch")

    Xm.data = <double *> Xa.data
    Xm.n = <int> Xa.shape[0]
    Xm.m = <int> Xa.shape[1]

    Ym.data = <double *> Ya.data
    Ym.n = <int> Ya.shape[0]
    Ym.m = <int> Ya.shape[1]

    cstats = mine_compute_cstats(&Xm, &Ym, &param)

    shape[0] = <np.npy_intp> cstats.n
    shape[1] = <np.npy_intp> cstats.m
    mica = np.PyArray_SimpleNewFromData(2, shape, np.NPY_DOUBLE,
                                        <void *> cstats.mic)
    np.PyArray_UpdateFlags(mica, mica.flags.num | np.NPY_OWNDATA)

    tica = np.PyArray_SimpleNewFromData(2, shape, np.NPY_DOUBLE,
                                        <void *> cstats.tic)
    np.PyArray_UpdateFlags(tica, tica.flags.num | np.NPY_OWNDATA)

    free(cstats)

    return mica, tica
