"""
- perform the ctypes conversion to call codingLib dynamic library in c++
- inspect the coding functions whether being performed correctly or not by
feeding a sample burst input and check the result (uppMac_str). 
"""
from ctypes import *
import numpy as np

#std::vector<uint8_t> ptrVec(std::uint8_t* ptr, int size) 

lib = cdll.LoadLibrary('/home/ctn008/tetraDMO-Receiver/codingLib.so') # class level loading lib

# C-type corresponding to numpy array 
ND_POINTER = np.ctypeslib.ndpointer(dtype=np.uint8, 
                                      ndim=1,
                                      flags="C")

def descramble(data, size, scramblingCode):
    lib.descramble_raw.argtypes = [ND_POINTER, c_size_t, ND_POINTER, c_size_t, c_size_t]
    lib.descramble_raw.restype = c_size_t
    res = np.array([0]*size, dtype='uint8')
    res_len = lib.descramble_raw(data, len(data), res, size, scramblingCode)
    print("descramble: ", res_len)
    return res

def deinterleave(data, K, a):
    lib.deinterleave_raw.argtypes = [ND_POINTER, c_size_t, ND_POINTER, c_size_t, c_size_t]
    lib.deinterleave_raw.restype = c_size_t
    res = np.array([0]*K, dtype='uint8')
    res_len = lib.deinterleave_raw(data, len(data), res, K, a)
    print("deinterleave: ", res_len)
    return res
    
def depuncture23(data, length):
    lib.depuncture23_raw.argtypes = [ND_POINTER, c_size_t, ND_POINTER, c_size_t]
    lib.depuncture23_raw.restype = c_size_t
    res = np.array([0]*(4*length*2//3), dtype='uint8')
    res_len = lib.depuncture23_raw(data, len(data), res, length)
    print("depuncture23: ", res_len)
    return res

def viterbiDecode1614(data):
    lib.viterbiDecode1614_raw.argtypes = [ND_POINTER, c_size_t, ND_POINTER]
    lib.viterbiDecode1614_raw.restype = c_size_t
    res = np.array([0]*(len(data)//4), dtype='uint8')
    res_len = lib.viterbiDecode1614_raw(data, len(data), res)
    print("viterbiDecode1614: ", res_len)
    return res

def checkCrc16Ccitt(data, length):
    lib.checkCrc16Ccitt_raw.argtypes = [ND_POINTER, c_size_t, c_size_t]
    lib.checkCrc16Ccitt_raw.restype = c_bool
    res_value = lib.checkCrc16Ccitt_raw(data, len(data), length)
    print("checkCrc16Ccitt: ", res_value)
    return res_value


burst_str = '000101000111101111111110000000000000000000000000000000000000000000000000000000000000001111111100001111111011001111011110011011010100000001110110111111111011100100101110101010111100000100111101000011010000101011100011000001100111001110110111000001100111110001110110110101010111101101001111000001000101100010100000011110111100001010100011010010001110100011001000000010111100001011111000000000010110111001010110100101110101101111111001010001000100101011010011001010001001000000001111111111111111111111111100000000' 
upMac_str = '1101000000001101110000000000000000000000000000000000000000000000000000100100000000000000001111101001010101110011001100110101110111000100000000011001000100001110000000000000000001000100'
bkn1 = np.array([0]*120, dtype='uint8')
bkn2 = np.array([0]*216, dtype='uint8')
for i in range(120):
    bkn1[i] = 0 if (burst_str[i+94] == '0') else 1 
for i in range(216):
    bkn2[i] = 0 if (burst_str[i+252] == '0') else 1 

bkn1 = descramble(bkn1, 120, 0x03)
bkn1 = deinterleave(bkn1, 120, 11)
bkn1 = depuncture23(bkn1, 120)
bkn1 = viterbiDecode1614(bkn1)

bkn2 = descramble(bkn2, 216, 0x03)
bkn2 = deinterleave(bkn2, 216, 101)
bkn2 = depuncture23(bkn2, 216)
bkn2 = viterbiDecode1614(bkn2)

print("CRC bkn1: ", checkCrc16Ccitt(bkn1, 76))
print("CRC bkn2: ", checkCrc16Ccitt(bkn2, 140))

res =''
for i in range(60):
    res += '1' if (bkn1[i]) else '0' 
for i in range(124):
    res += '1' if (bkn2[i]) else '0' 
if res == upMac_str:
    print("Correct output.")
else:
    print("Something wrong. Please investigate.")
    
# call function
#outSize = lib.descramble_raw(inData, inData.size, outData, inData.size, 0x03)


"""
    def __init__(self):
        self.vector = Vector.lib.new_vector()  # pointer to new vector

    def __del__(self):  # when reference count hits 0 in Python,
        Vector.lib.delete_vector(self.vector)  # call C++ vector destructor

    def __len__(self):
        return Vector.lib.vector_size(self.vector)

    def __getitem__(self, i):  # access elements in vector at index
        if 0 <= i < len(self):
            return Vector.lib.vector_get(self.vector, c_int(i))
        raise IndexError('Vector index out of range')

    def __repr__(self):
        return '[{}]'.format(', '.join(str(self[i]) for i in range(len(self))))

    def push(self, i):  # push calls vector's push_back
        Vector.lib.vector_push_back(self.vector, c_int(i))

    def foo(self, filename):  # foo in Python calls foo in C++
        return Vector.lib.foo(self.vector, c_char_p(filename))

# Python string
py_string = "Hello, World!"

# Convert Python string to bytes
byte_string = py_string.encode('utf-8')

# Create a c_char_p instance
c_string = c_char_p(byte_string)

# Print the c_char_p string
print(c_string)          # Output: c_char_p(139946046266032)
print(c_string.value)    # Output: b'Hello, World!'

a=Vector()
a.push(22)
a.push(20)
print(a)
result = a.foo('/home/ctn008/tetraDMO-Receiver/file.txt'.encode('utf-8'))
print("line count in foo: ", result)
"""
