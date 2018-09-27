#!/bin/python

import subprocess as sub
import time
import sys

n = int(sys.argv[1])

for name in ["rowO0", "rowO2", "colO0", "colO2"]:

    start = time.time()

    for i in range(n):
        sub.call(["./" + name])

    elapsed = time.time() - start

    print("Time " + name + ":", elapsed)
