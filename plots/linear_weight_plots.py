#!/usr/bin/env python2

import math
import matplotlib.pyplot as plt
import numpy as np
import csv

triples = [('../data/saturnin-linear-weight-full.txt', 's-', 'b', 'Saturnin'),
           ('../data/spongent-linear-weight-full.txt', 'X-', 'y', 'Spongent'),
           ('../data/xoodoo-linear-trail-partial.txt', 'o-', 'g', 'Xoodoo')]

for t in triples:
    linear_trail_weights = []
    number_of_trails = []
    with open(t[0], 'r') as csvfile:
        plots = csv.reader(csvfile, delimiter=',')
        for row in plots:
		linear_trail_weights.append(int(row[0]))
		number_of_trails.append(float(row[2]))
    missing_pairs = [(x, 0) for x in range(linear_trail_weights[0], linear_trail_weights[-1]+1) if x not in linear_trail_weights]
    full_weights = zip(linear_trail_weights, number_of_trails) + missing_pairs
    full_weights.sort()
    linear_trail_weights, number_of_trails = zip(*full_weights)
    plt.plot(linear_trail_weights, np.log2(np.cumsum(number_of_trails)), t[1], label=t[3], color=t[2], drawstyle='steps-post', markersize=2)

plt.xticks(np.arange(0,36,5))
plt.xlim(0, 35)
plt.ylim(0, 80)
plt.xlabel('Linear Approximation Weight', fontsize=16)
plt.ylabel('Log2(Number of Linear Approximations)', fontsize=16)
plt.legend()
plt.grid(b=True, which='major')
plt.savefig('linear_weights.pdf', bbox_inches='tight')
