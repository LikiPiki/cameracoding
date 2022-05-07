#!/usr/bin/python
# -*- coding: utf-8 -*-

import matplotlib.pyplot as plt

def filter_150_mb(x):
	if x >= 20000:
		return False

	return True

# My results
x1 = [
	651427.75,
	75591424 / 1024,
	39849243 / 1024,
	20515179 / 1024,
	10804527 / 1024,
	98304 / 8,
	70655 / 8
];

y1 = [
	50.58,
	47.9181,
	45.4749,
	43.3008,
	41.4386,
	40.04,
	39.3884,
]

plt.plot(x1, y1, label=u'Предлагаемый кодек')

x2 = [
	255166.666666667,	
	59166.6666666667,
	38100,
	25100,
	19133.3333333333,
	12933.3333333333,
	9400,
]

y2 = [
	49.488,
	46.7792,
	45.5144,
	43.8717,
	42.5165,
	39.9152,
	36.3122,
]

plt.plot(x2, y2, label=u'MJPEG')
plt.legend(loc='upper left')
 
plt.xlabel(u'Битрейт (в килобайтах)')
plt.ylabel(u'Y (PSNR)')

plt.xlim([0,250000])
 
plt.show()