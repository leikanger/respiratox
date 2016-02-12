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
    * ? Stress test for multiple messages in close succession - [has never failed]
### Decoding messages
    * X Messages are separated by \n
    * X Correct message decoding: One message gives three numbers
    * X Values for different axes are separated by \t
    * X Correct value:  Message gives 3 values for normal messages
    * - Correck value:  Message also handles short or errorenous messages
    * X Correct values: X,Y,Z - correct decoding of 3 values
    * X Strange message or zero does not chrash execution
    * X Correct decoding: very large numbers
### Structuring messages and data structures
Make a design for saving data in data structures (on heap?), so that e.g. all
x-values can be taken out. Create design for how to save values.
    * 

#TODO : Update MOCK objec, so that different messages gives different xvalues e.g.


Decoding message and saving result
------------------------------------------------
