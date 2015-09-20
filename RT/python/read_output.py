#!/usr/bin/python

import struct
import numpy as np
import pylab as pl

nx = 100
ny = 100

fid = open("../test/result.r", 'rb')

raw_data = fid.read(8)

num = struct.unpack( 'i', raw_data[0:4] )
nx = num[0]
num = struct.unpack( 'i', raw_data[4:8] )
ny = num[0]

data = np.zeros(nx*ny)

raw_data = fid.read()

#for k in range(len(raw_data)/4):
for k in range(nx*ny):
   num = struct.unpack( 'f', raw_data[k*4:k*4+4] )
   data[k] = num[0]

fid.close()

data = np.reshape(data, (ny,nx))

pl.gray()
pl.imshow(data, interpolation='nearest', origin='lower')
pl.show()
