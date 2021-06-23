#!/usr/bin/env python2

import csv
import argparse

parser = argparse.ArgumentParser()
parser.add_argument("filename", help="Input CSV file containing partition.")
args = parser.parse_args()

with open(args.filename, 'r') as csvfile:
    plots = csv.reader(csvfile, delimiter=',')
    dict = {}
    for row in plots:
        if row[0] in dict:
            dict[row[0]].append("(%s x %s)" % (row[1], row[2]))
        else:
            dict[row[0]] = ["(%s x %s)" % (row[1], row[2])]

print '\\begin{table}[]'
print '\\begin{tabular}{ll}'
print 'Weight & (#subsets x cardinality) \\\\'
print '\\hline'
for key, value_list in dict.iteritems():
    print key + ' &',
    for value in value_list:
        print value,
    print '\\\\'
print '\\end{tabular}'
print '\\end{table}'
