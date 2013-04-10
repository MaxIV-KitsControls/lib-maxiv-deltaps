#!/usr/bin/python

import numpy as np

x = np.linspace(0,2*np.pi,100)

wf = np.sin(x)

for sample in wf:
    print '%f' % sample
