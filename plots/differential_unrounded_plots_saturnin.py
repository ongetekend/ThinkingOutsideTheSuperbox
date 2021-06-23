#!/usr/bin/env python2

import math
import matplotlib.pyplot as plt
import numpy as np
import csv

triples = [('../data/saturnin-differential-superbox-unrounded.txt', '-', 'r', 'Differentials'), 
	   ('../data/saturnin-differential-trail-superbox.txt', '-', 'b', 'Differential Trails')]

w = []
c = []
with open(triples[0][0], 'r') as csvfile:
    plots = csv.reader(csvfile, delimiter=',')
    for row in plots:
        if int(row[1]) != 0 and int(row[0]) != 2**16:
	    w.append((2**16) / float(row[0]))
	    c.append(float(row[1]))
    pairs = zip(w, c)
    pairs.sort()
    w, c = zip(*pairs)
    plt.plot(np.log2(w), np.log2(np.cumsum(c)), triples[0][1], label=triples[0][3], color=triples[0][2], drawstyle='steps-post', markersize=2)

w = []
c = []
with open(triples[1][0], 'r') as csvfile:
    rows = csv.reader(csvfile, delimiter=',')
    for row in rows:
        if int(row[0]) != 0 and int(row[2]) != 0:
	    w.append(int(row[0]))
	    c.append(float(row[2]))
    missing_pairs = [(x, 0) for x in range(w[0], w[-1]+1) if x not in w]
    full_weights = zip(w, c) + missing_pairs
    full_weights.sort()
    w, c = zip(*full_weights)
    plt.plot(w, np.log2(np.cumsum(c)), triples[1][1], color=triples[1][2], label=triples[1][3], drawstyle='steps-post', markersize=2)

plt.xticks(np.arange(0,24,2))
plt.xlim(0, 24)
plt.ylim(0, 40)
plt.xlabel('Weight', fontsize=16)
plt.ylabel('Log2(Count)', fontsize=16)
plt.title('Saturnin')
plt.legend(loc='upper left')
plt.grid(b=True, which='major')
plt.savefig('differential_saturnin_superbox_unrounded.pdf', bbox_inches='tight')
