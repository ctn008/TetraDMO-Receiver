"""
Embedded Python Blocks:

receive dmoBurst and perform lowerMac functions
"""

import numpy as np
from gnuradio import gr
import ctypes

codingLib = ctypes.cdll.LoadLibrary('/home/ctn008/tetraDMO-Receiver/codingLib.so')
codingLib.descramble.argtypes = [ctypes.POINTER(ctypes.c_ubyte), ctypes.c_int, ctypes.c_int]
codingLib.descramble.restype = ctypes.POINTER(ctypes.c_ubyte)

c_ubyte_p = ctypes.POINTER(ctypes.c_ubyte)
bkn1 = np.array([0]*510, dtype= 'uint8')
bkn1_p = bkn1.ctypes.data_as(c_ubyte_p)
codingLib.descramble(codingLib.ptrVec(bkn1_p,520), 120, 0x0003) 

