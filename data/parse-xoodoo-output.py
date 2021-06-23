#!/usr/bin/env python3

from collections import defaultdict
import argparse
import re

parser = argparse.ArgumentParser()
parser.add_argument("-f", "--file", help="File to parse", default="DC-Xoodoo-3r.txt")
args = parser.parse_args()

f = open(args.file, 'r')
text = f.read()

digits = re.findall('\d+', text)
decomposed_digits = list(zip(*[iter(digits)]*8))

d = defaultdict(int)
for chunk in decomposed_digits:
	d[int(chunk[1])] += 128*2**(int(chunk[3]) + int(chunk[7]))

for t in sorted(d.items()):
	print(*t, sep=",0,")
