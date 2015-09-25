#!/usr/bin/python

import struct
import numpy as np
import pylab as pl

fid = open("../test/result.r", 'rb')

raw_data = fid.read(8)

num = struct.unpack( 'i', raw_data[0:4] )
nx = num[0]
num = struct.unpack( 'i', raw_data[4:8] )
ny = num[0]

data_red = np.zeros(nx*ny)

raw_data = fid.read()
fid.close()

for k in range(nx*ny):
   num = struct.unpack( 'f', raw_data[k*4:k*4+4] )
   data_red[k] = num[0]

data_red = np.reshape(data_red, (ny,nx))


fid = open("../test/result.g", 'rb')

raw_data = fid.read(8)

num = struct.unpack( 'i', raw_data[0:4] )
nx = num[0]
num = struct.unpack( 'i', raw_data[4:8] )
ny = num[0]

data_green = np.zeros(nx*ny)

raw_data = fid.read()
fid.close()

for k in range(nx*ny):
   num = struct.unpack( 'f', raw_data[k*4:k*4+4] )
   data_green[k] = num[0]

data_green = np.reshape(data_green, (ny,nx))

fid = open("../test/result.b", 'rb')

raw_data = fid.read(8)

num = struct.unpack( 'i', raw_data[0:4] )
nx = num[0]
num = struct.unpack( 'i', raw_data[4:8] )
ny = num[0]

data_blue = np.zeros(nx*ny)

raw_data = fid.read()
fid.close()

for k in range(nx*ny):
   num = struct.unpack( 'f', raw_data[k*4:k*4+4] )
   data_blue[k] = num[0]

data_blue = np.reshape(data_blue, (ny,nx))

#image = np.array([data_red, data_green, data_blue])
image = np.zeros((nx,ny,3))
image[:,:,0] = data_red
image[:,:,1] = data_green
image[:,:,2] = data_blue

#pl.gray()
pl.imshow(image, interpolation='nearest', origin='lower')
pl.show()
