import ctypes
import time


testlib = ctypes.CDLL('./matsqure.so')
testlib.read("./mat10000.dat")
start = time.time()
testlib.multi()
end = time.time()
print(end - start)
