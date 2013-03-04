import numpy as np
from .mine import MINE


def pearson(x, y):
    """Returns the Pearson correlation between x and y.
    """

    xa = np.asarray(x, dtype=np.float)
    ya = np.asarray(y, dtype=np.float)
    if xa.shape[0] != ya.shape[0]:
        raise ValueError("x, y: shape mismatch")
    
    xc = xa - np.mean(xa)
    yc = ya - np.mean(ya)
    a = np.sum(xc*yc)
    b = np.sqrt(np.sum(xc**2))
    c = np.sqrt(np.sum(yc**2))
    
    return a / (b * c)


def minestats(x, y, alpha=0.6, c=15):
    """Convenience function returning a dictionary containing
    the MINE statistics: 'mic' (strength), 'nonlinearity' (mic-r^2),
    'mas' (non-monotonicity), 'mev' (functionality), 'mcn' (complexity, eps=0), 
    'mcn_general' (complexity, eps=1-MIC) and 'pearson'.

    :Parameters:
        alpha : float (0, 1.0]
            the exponent in B(n) = n^alpha
        c : float (> 0)
            determines how many more clumps there will be than
            columns in every partition. Default value is 15, meaning
            that when trying to draw x grid lines on the x-axis,
            the algorithm will start with at most 15*x clumps

    :Returns:
        dictionary : {'mic': VALUE, 'nonlinearity': VALUE, 'mas': VALUE,
                      'mev': VALUE, 'mcn': VALUE, 'mcn_general': VALUE,
                      'pearson': VALUE}
    """

    m = MINE(alpha=alpha, c=c)
    m.compute_score(x, y)
    ret = {}
    ret['mic'] = m.mic()
    ret['mas'] = m.mas()
    ret['mev'] = m.mev()
    ret['mcn'] = m.mcn(eps=0)
    ret['mcn_general'] = m.mcn_general()
    ret['pearson'] = pearson(x, y)
    ret['nonlinearity'] = ret['mic'] - ret['pearson']**2
    
    return ret




