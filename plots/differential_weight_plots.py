#!/usr/bin/env python2

import math
import matplotlib.pyplot as plt
import numpy as np
import csv

triples = [('../data/saturnin-differential-full.txt', 's-', 'b', 'Saturnin'),
           ('../data/spongent-differential-full.txt', 'X-', 'y', 'Spongent'),
           ('../data/xoodoo-differential-partial.txt', 'o-', 'g', 'Xoodoo')]

for t in triples:
    w = []
    c = []
    with open(t[0], 'r') as csvfile:
        plots = csv.reader(csvfile, delimiter=',')
        for row in plots:
            if int(row[1]) != 0:
                w.append(int(row[0]))
                c.append(float(row[1]))
    missing_pairs = [(x, 0) for x in range(w[0], w[-1]+1) if x not in w]
    full_weights = zip(w, c) + missing_pairs
    full_weights.sort()
    w, c = zip(*full_weights)
    plt.plot(w, np.log2(np.cumsum(c)), t[1], label=t[3], color=t[2], drawstyle='steps-post', markersize=2)

plt.xticks(np.arange(0,36,5))
plt.xlim(0, 35)
plt.ylim(0, 80)
plt.xlabel('Differential Weight', fontsize=16)
plt.ylabel('Log2(Number of Differentials)', fontsize=16)
plt.legend()
plt.grid(b=True, which='major')
plt.savefig('differential_weights.pdf', bbox_inches='tight')
