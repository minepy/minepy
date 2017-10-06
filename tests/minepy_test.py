import unittest
import numpy as np
from numpy.testing import *
from minepy import MINE


class TestFunctions(unittest.TestCase):

    def setUp(self):
        self.mine = MINE(alpha=0.6, c=15)

    def build_const(self, n):
        x = np.linspace(0, 1, n)
        y = np.zeros(n)
        return x, y

    def build_linear(self, n):
        x = np.linspace(0, 1, n)
        return x, x

    def build_sine(self, n):
        x = np.linspace(0, 1, n)
        return x, np.sin(8*np.pi*x)

    def build_exp(self, n):
        x = np.linspace(0, 10, n)
        return x, 2**x

    def test_const(self):
        x, y = self.build_const(1000)
        self.mine.compute_score(x, y)
        assert_almost_equal(self.mine.mic(), 0., 4)
        assert_almost_equal(self.mine.mas(), 0., 4)
        assert_almost_equal(self.mine.mev(), 0., 4)
        assert_almost_equal(self.mine.mcn(), 2., 4)
        assert_almost_equal(self.mine.mcn_general(), 2., 4)

    def test_linear(self):
        x, y = self.build_linear(1000)
        self.mine.compute_score(x, y)
        assert_almost_equal(self.mine.mic(), 1., 4)
        assert_almost_equal(self.mine.mas(), 0., 4)
        assert_almost_equal(self.mine.mev(), 1., 4)
        assert_almost_equal(self.mine.mcn(), 2., 4)
        assert_almost_equal(self.mine.mcn_general(), 2., 4)

    def test_linear(self):
        x, y = self.build_linear(1000)
        self.mine.compute_score(x, y)
        assert_almost_equal(self.mine.mic(), 1., 4)
        assert_almost_equal(self.mine.mas(), 0., 4)
        assert_almost_equal(self.mine.mev(), 1., 4)
        assert_almost_equal(self.mine.mcn(), 2., 4)
        assert_almost_equal(self.mine.mcn_general(), 2., 4)

    def test_sine(self):
        x, y = self.build_sine(1000)
        self.mine.compute_score(x, y)
        assert_almost_equal(self.mine.mic(), 1., 4)
        assert_almost_equal(self.mine.mas(), 0.875, 3)
        assert_almost_equal(self.mine.mev(), 1., 4)
        assert_almost_equal(self.mine.mcn(), 4., 4)
        assert_almost_equal(self.mine.mcn_general(), 4., 4)

    def test_exp(self):
        x, y = self.build_exp(1000)
        self.mine.compute_score(x, y)
        assert_almost_equal(self.mine.mic(), 1., 4)
        assert_almost_equal(self.mine.mas(), 0., 4)
        assert_almost_equal(self.mine.mev(), 1., 4)
        assert_almost_equal(self.mine.mcn(), 2., 4)
        assert_almost_equal(self.mine.mcn_general(), 2., 4)


if __name__ == '__main__':
    #unittest.main()
    suite = unittest.TestLoader().loadTestsFromTestCase(TestFunctions)
    unittest.TextTestRunner(verbosity=2).run(suite)
    
