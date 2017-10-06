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

    mine_score *mine_compute_score (mine_problem *prob,
                                    mine_parameter *param) nogil
    char *mine_check_parameter(mine_parameter *param) nogil
    double mine_mic (mine_score *score) nogil
    double mine_mas (mine_score *score) nogil
    double mine_mev (mine_score *score) nogil
    double mine_mcn (mine_score *score, double eps) nogil
    double mine_mcn_general (mine_score *score) nogil
    double mine_tic (mine_score *score, int norm) nogil
    double mine_gmic (mine_score *score, double p) nogil
    void mine_free_score (mine_score **score) nogil
    int EST_MIC_APPROX
    int EST_MIC_E

    # convenience structures and functions
    ctypedef struct mine_matrix:
        double *data
        int n
        int m

    ctypedef struct mine_pstats:
        double *mic
        double *tic
        int n

    ctypedef struct mine_cstats:
        double *mic
        double *tic
        int n
        int m

    mine_pstats *mine_compute_pstats(mine_matrix *X, mine_parameter *param) nogil
    mine_cstats *mine_compute_cstats(mine_matrix *X, mine_matrix *Y,
                                     mine_parameter *param) nogil