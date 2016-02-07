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
    * Verify serial path / system path to serial interface
    * Constructor
        - empties serial buffer
### Core functionality
    * Receive data -- echo message > [outserial-system-filepath-path]
        and receive through serial interface.
    * Send data -- verify with 'head -1 /dev/pts/RECEIVE_PATH'
    * Send and receive from objects from class Serial => right message recievd
    * When two messages comes right after eachother, both are correctly received
    * 

Decoding message and saving result
------------------------------------------------
### Decode message
    * Message is divided into correct coordinates
    * 
