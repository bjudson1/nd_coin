#!/usr/bin/env python2.7

import os
import sys
import string
import itertools

# Constants

LOWER = 100001
UPPER = 1000000

def isPrime(number):
	if number % 2 == 0:
		return 0

	for num in range(3, (number/2), 2):
		if number % num == 0:
			return 0

	return 1;

primes = []
for num in range(LOWER, UPPER, 2):
	if isPrime(num) == 1:
		primes.append(num)

f = open("primes.txt")
f.write(primes)
f.close()