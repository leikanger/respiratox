#! /usr/bin/ipython

import socket           # import socket module

s = socket.socket();    # create socket object
host = socket.gethostname(); # Get local maschine name
port = 13               # reserve a port for my service (from TCPserver in beaglebone)

s.connect((host, port))

for i in range(0,10):
    print s.recv(1024);

