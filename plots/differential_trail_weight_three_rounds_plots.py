#!/usr/bin/env python2

import math
import matplotlib.pyplot as plt
import numpy as np
import csv

triples = [('../data/saturnin-differential-trail-three-rounds_total.txt', 's-', 'b', 'Saturnin'),
           ('../data/xoodoo-differential-trails-three-rounds.txt', 'o-', 'g', 'Xoodoo')]

for t in triples:
    w = []
    c = []
    with open(t[0], 'r') as csvfile:
        rows = csv.reader(csvfile, delimiter=',')
        for row in rows:
            if int(row[2]) != 0:
                w.append(int(row[0]))
                if t[3] == "Saturnin" and float(row[2]) != 1:
                    c.append(4*float(row[2]))
                else:
                    c.append(float(row[2]))
    missing_pairs = [(x, 0) for x in range(w[0], w[-1]+1) if x not in w]
    full_weights = zip(w, c) + missing_pairs
    full_weights.sort()
    w, c = zip(*full_weights)
    plt.plot(w, np.log2(np.cumsum(c)), t[1], color=t[2], label=t[3], drawstyle='steps-post', markersize=2)

plt.xticks(np.arange(0,51,5))
plt.xlim(0, 50)
plt.ylim(0, 60)
plt.xlabel('Differential Trail Weight', fontsize=16)
plt.ylabel('Log2(Number of Trails)', fontsize=16)
plt.legend()
plt.grid(b=True, which='major')
plt.savefig('differential_trail_weights_three_rounds.pdf', bbox_inches='tight')
