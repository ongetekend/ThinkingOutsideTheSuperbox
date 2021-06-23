#!/usr/bin/env python2

import math
import matplotlib.pyplot as plt
import numpy as np
import csv

triples = [('../data/saturnin-hamming-full.txt', 's-', 'b', 'Saturnin'),
           ('../data/spongent-hamming-full.txt', 'X-', 'y', 'Spongent'),
           ('../data/aes-hamming-full.txt', 'D-', 'r', 'Rijndael'),
           ('../data/xoodoo-hamming-partial.txt', 'o-', 'g', 'Xoodoo'),
           ('../data/identity-hamming.full.txt', '*-', 'k', 'Identity')]
for t in triples:
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
    plt.plot(w, np.log2(np.cumsum(c)), t[1], label=t[3], color=t[2], drawstyle='steps-post', markersize=2)
plt.xticks(np.arange(0,27,2))
plt.xlim(0, 26)
plt.ylim(0, 80)
plt.xlabel('Bit Weight', fontsize=16)
plt.ylabel('Log2(Number of States)', fontsize=16)
plt.legend()
plt.grid(b=True, which='major')
plt.savefig('bit_weights.pdf', bbox_inches='tight')
