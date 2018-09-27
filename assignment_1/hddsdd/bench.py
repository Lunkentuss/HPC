#!/bin/python

import subprocess as sub
import time
import sys

if len(sys.argv) < 2:
    print("Forgot number of iterations")
    exit()

n = int(sys.argv[1])


start = time.time()

for i in range(n):
    sub.call(["./main_write"])

elapsed = time.time() - start

print("Time " + "write" + ":", elapsed)

print("===============")

for i in range(n):
    sub.call(["./main_read"])

elapsed = time.time() - start

print("Time " + "read" + ":", elapsed)
