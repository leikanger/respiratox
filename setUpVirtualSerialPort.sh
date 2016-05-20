
# ABOUT: Set up virtual serial port for unit tests of respiratoX.
#   - creates a virtual serial port in /dev/?, and this path needs to be
#       written into the file ./pathToVirtualSerialPorts.h
#
# DEP: apitude install socat !!!
# A virtual serial port can be set up using [socat] terminal command. 
socat -d -d pty,raw,echo=0 pty,raw,echo=0  &
