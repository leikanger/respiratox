# respiratox
Desktop client for receiving data from RespirationRate measurements.

# TDD prerequisites
The boost unit test framework is used, and a virtual serial port must be set
up. This has to be done after every boot on a new system, and can be done with
the convenience script ./setUpVirtualSerialPort.sh
-> Read the output from this script, as one run sets up a pipe from one serial
port (/dev/pts/? - address) to an other. 
I have not automized it yet, and these two paths needs to be written into the
header file pathToVirtualSerialPorts.h
