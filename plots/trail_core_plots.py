#!/usr/bin/env python2

import math
import matplotlib.pyplot as plt
import numpy as np
import csv

triples = [('../data/saturnin-trail-core-full.txt', 's-', 'b', 'Saturnin'),
           ('../data/spongent-trail-core-full.txt', 'X-', 'y',  'Spongent'), 
           ('../data/aes-trail-core-full.txt', 'D-', 'r', 'Rijndael'), 
           ('../data/xoodoo-trail-core-partial.txt', 'o-', 'g', 'Xoodoo')]

for t in triples:
# Smallest weight for Rijndael is 6
# Smallest weight for Spongent is 2
# Smallest weight for Saturnin is 2
# Smallest weight for Xoodoo is 2
    w = []
    c = []
    with open(t[0], 'r') as csvfile:
        rows = csv.reader(csvfile, delimiter=',')
        for row in rows:
            if int(row[2]) != 0:
                w.append(int(row[0]))
                c.append(float(row[2]))
    missing_pairs = [(x, 0) for x in range(w[0], w[-1]+1) if x not in w]
    full_weights = zip(w, c) + missing_pairs
    full_weights.sort()
    w, c = zip(*full_weights)
    plt.plot(w, np.log2(np.cumsum(c)), t[1], color=t[2], label=t[3], drawstyle='steps-post', markersize=2)

plt.xticks(np.arange(0,51,5))
plt.xlim(0, 50)
plt.ylim(0, 140)
plt.xlabel('Trail Core Weight', fontsize=16)
plt.ylabel('Log2(Number of Trail Cores)', fontsize=16)
plt.legend()
plt.grid(b=True, which='major')
plt.savefig('trail_core_weights.pdf', bbox_inches='tight')
