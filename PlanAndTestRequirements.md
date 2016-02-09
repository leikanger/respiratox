Plan
=====

Requirements for RespiraToX
================================================
Able to receive messages with accelerometer data
------------------------------------------------
    * Must be able to receive data in the form [[[dataX|dataY|dataX\n]]]
        - where | is the chosen separator sign, [[[and]]] is only to show
          where the above example starts and stops and \n is the endline that
          separates different messages.
    * No requirements with regard to efficancy
    * 
The network protocol is undecided
------------------------------------------------
This focus on having clear separation between code and signal protocol:
remember to have a strong focus on interface and dependency inversion principle..

TDD - Test Plan
================================================
More detailed than the above req.spec., and also a plan for unit tests and
code development (TDD). 

ASIO and core functionality
---------------------------
### Construction and destruction
    * X Verify serial path / system path to serial interface
    * X Constructor
    * X Test for vitual serial ports: verify empty buffer..
### Core functionality
    * X Receive data -- echo message > [outserial-system-filepath-path]
        and receive through serial interface.
    * X Send data -- verify with 'head -1 /dev/pts/RECEIVE_PATH'
    * X Send and receive from objects from class Serial => right message recievd
    * - When two messages comes right after eachother, both are correctly received
### Sensor-Dummy
    * X Create Test-dummy for sending and also documenting how arduino should
        behave
## Receiving messages
    * - Stress test for multiple messages in close succession
### Decoding messages
    * X Messages are separated by \n
    * - Values for different axes are separated by \t
    * - Correct message decoding: One message gives three numbers
    * - Correct value: X Correct decoding for one value (eg. X axis)
    * - Correct values: X,Y,Z 
    * - Correct decoding: 0 and 0.0
    * - Correct decoding: very large numbers

Decoding message and saving result
------------------------------------------------
### Decode message
    * Message is divided into correct coordinates
    * 
