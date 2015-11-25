#! /usr/bin/ipython
# -*- coding: utf-8 -*-

import socket           # import socket module
import struct           # struct, for packing and unpacking data

import continuousPlotter # testing continuousPlotter with client

class clientTCP:
    """A client for TCP connections"""

    def __init__(self):
        self.s = socket.socket();    # create socket object
        host = socket.gethostname(); # Get local maschine name
        port = 12345               # reserve a port for my service (from TCPserver in beaglebone)

        self.s.connect((host, port))
        # Plan: lag tråd som tar i mot data og skriv til en mdl.var.
        # Vektor med verdier (mdl.var) kan hentes ut ved self.get() funk. eller noke
    
    def __del__(self):
        self.s.close()

    def get(self):
        N = 30; # Ti verdier om gangen..
        #s = struct.Struct(str(N)+'h') # Package 10 'h' -- that is signed short - 16 bit
        ansver = self.s.recv(1024);
        #print ansver
        return struct.unpack(str(N)+'h', self.s.recv(1024))



if __name__ == "__main__":
    plotter  = continuousPlotter.continuousPlotter()
    provider = clientTCP()
    for i in range(0,1000):
        values = provider.get()
        plotter.plotNewValues(values)
