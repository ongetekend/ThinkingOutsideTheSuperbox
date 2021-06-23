#!/usr/bin/env python2

import sys
import csv
import matplotlib.pyplot as plt
from decimal import Decimal
from math import log
from collections import defaultdict

with open(sys.argv[1], 'rb') as csvfile:
    reader = csv.reader(csvfile)
    correlation_numerators = []
    number_of_linear_approximations = []
    for row in reader:
            correlation_numerators.append(Decimal(row[0]))
            number_of_linear_approximations.append(int(row[1]))

tuples = [(int(2*(16 - log(x, 2))), y) for x, y in zip(correlation_numerators, number_of_linear_approximations) if (x != 0 and y != 0)]

t_dict = defaultdict(tuple)
for t in tuples:
    if t[0] in t_dict:
        t_dict[t[0]] += t[1]
    else:
        t_dict[t[0]] = t[1]

for key in t_dict:
    print("{},{}".format(key, t_dict[key]))
