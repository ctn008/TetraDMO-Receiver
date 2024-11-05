from ctypes import *

class Vector(object):
    lib = cdll.LoadLibrary('/home/ctn008/tetraDMO-Receiver/vector_python_lib.so') # class level loading lib
    lib.new_vector.restype = c_void_p
    lib.new_vector.argtypes = []
    lib.delete_vector.restype = None
    lib.delete_vector.argtypes = [c_void_p]
    lib.vector_size.restype = c_int
    lib.vector_size.argtypes = [c_void_p]
    lib.vector_get.restype = c_int
    lib.vector_get.argtypes = [c_void_p, c_int]
    lib.vector_push_back.restype = None
    lib.vector_push_back.argtypes = [c_void_p, c_int]
    lib.foo.restype = c_int
    lib.foo.argtypes = [c_void_p]

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
