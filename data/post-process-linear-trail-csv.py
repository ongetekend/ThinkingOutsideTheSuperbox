import csv
import matplotlib.pyplot as plt
from decimal import Decimal
from math import log
import sys

with open(sys.argv[1], 'rb') as csvfile:
    reader = csv.reader(csvfile)
    correlations = []
    number_of_masks = []
    for row in reader:
            correlations.append(Decimal(row[0])/2**Decimal(row[1]))
            number_of_masks.append(Decimal(row[2]))

tuples = [(2*log(x, 2), log(y, 2)) for x, y in zip(correlations, number_of_masks) if (x != 0 and y != 0)]

plt.plot(zip(*tuples)[0], zip(*tuples)[1], 'ro')
plt.show()
