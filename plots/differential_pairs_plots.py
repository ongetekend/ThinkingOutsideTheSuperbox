#!/usr/bin/env python2

import math
import matplotlib.pyplot as plt
import numpy as np
import csv

triples = [('../data/saturnin-differential-pairs.txt', '-', 'r', 'Differentials'), 
	   ('../data/saturnin-differential-trail-superbox.txt', '-', 'b', 'Differential Trails')]

p = []
c = []
with open(triples[0][0], 'r') as csvfile:
    plots = csv.reader(csvfile, delimiter=',')
    for row in plots:
        if int(row[1]) != 0 and int(row[0]) != 2**16:
	    p.append((2**16) / float(row[0]))
	    c.append(float(row[1]))
    pairs = zip(p, c)
    pairs.sort()
    p, c = zip(*pairs)
    plt.plot(np.log2(p), np.log2(np.cumsum(c)), triples[0][1], label=triples[0][3], color=triples[0][2], drawstyle='steps-post', markersize=2)

w = []
c = []
with open(triples[1][0], 'r') as csvfile:
    rows = csv.reader(csvfile, delimiter=',')
    for row in rows:
        if int(row[0]) != 0 and int(row[2]) != 0:
	    w.append(int(row[0]))
	    c.append(float(row[2])*(2**(16-int(row[0]))))
    missing_pairs = [(x, 0) for x in range(w[0], w[-1]+1) if x not in w]
    full_weights = zip(w, c) + missing_pairs
    full_weights.sort()
    w, c = zip(*full_weights)
    plt.plot(w, np.log2(np.cumsum(c)), triples[1][1], color=triples[1][2], label=triples[1][3], drawstyle='steps-post', markersize=2)

plt.xticks(np.arange(0,26,2))
plt.xlim(0, 25)
plt.ylim(0, 35)
plt.xlabel('Restriction Weight', fontsize=16)
plt.ylabel('Log2(Number of Ordered Pairs)', fontsize=16)
plt.legend(loc='upper left')
plt.grid(b=True, which='major')
plt.savefig('differential_pairs.pdf', bbox_inches='tight')
