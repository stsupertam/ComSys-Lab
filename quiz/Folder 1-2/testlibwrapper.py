import ctypes
import time


midterm = ctypes.CDLL('./midterm_2.so')
midterm.read("./data")
midterm.sort()
