#!/usr/bin/python

import struct
import numpy as np
import pylab as pl

def read_data_size( file_name ):
   fid = open( file_name, 'rb')
   raw_data = fid.read(8)
   num = struct.unpack( 'i', raw_data[0:4] )
   nx  = num[0]
   num = struct.unpack( 'i', raw_data[4:8] )
   ny  = num[0]
   fid.close()
   return [nx, ny]

def read_data( file_name ):
   fid = open( file_name, 'rb')
   raw_data = fid.read(8)
   num = struct.unpack( 'i', raw_data[0:4] )
   nx  = num[0]
   num = struct.unpack( 'i', raw_data[4:8] )
   ny  = num[0]
   data = np.zeros(nx*ny)
   raw_data = fid.read()
   fid.close()
   for k in range(nx*ny):
      num = struct.unpack( 'f', raw_data[k*4:k*4+4] )
      data[k] = num[0]
   data = np.reshape(data, (ny,nx))
   return data


data_red   = read_data( "../test/result.r" )
data_green = read_data( "../test/result.g" )
data_blue  = read_data( "../test/result.b" )

# normalize
data_red   /= data_red.max()
data_green /= data_green.max()
data_blue  /= data_blue.max()

nxy = read_data_size( "../test/result.r" )

print "size = [%d, %d]" % (nxy[0], nxy[1])

# image = np.array([data_red, data_green, data_blue])
image = np.zeros((nxy[1],nxy[0],3))
image[:,:,0] = data_red
image[:,:,1] = data_green
image[:,:,2] = data_blue

# pl.gray()
pl.imshow(image, interpolation='nearest', origin='lower')
pl.show()
