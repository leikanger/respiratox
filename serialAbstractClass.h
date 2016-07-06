#include <string>
#include <iostream>
#include <boost/asio.hpp>
#include <fstream>
#include <vector>

#ifndef SERIALABSTRACTCLASS_H
#define SERIALABSTRACTCLASS_H

class SerialAbstractClass {
public:
    //SerialAbstractClass(const std::string& port, unsigned int baud_rate =9600) 
    SerialAbstractClass() { }

    virtual std::string read()  =0;
    virtual void readIntoBufferArg(std::string* pTextBuffer) =0;
    virtual int write_message(std::string pTextBuffer) =0;
};

#endif
