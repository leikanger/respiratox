#include "serialAbstractClass.h"
#include <string>
#include <iostream>
#include <boost/asio.hpp>
#include <fstream>
#include <vector>

#ifndef SERIALBOOST_H
#define SERIALBOOST_H

/************/
/** SerialBOOST **/
/************/
class SerialBOOST : public SerialAbstractClass {
public:
    SerialBOOST(const std::string& port, 
    /**/        unsigned int baud_rate =9600, 
    /**/        boost::system::error_code* pec =nullptr);
    ~SerialBOOST();

    // TODO før inn const på dei rette funk'er!
    std::string read();
    void readIntoBufferArg(std::string* pTextBuffer);
    int write_message(std::string pTextBuffer);
private:
    int write_some(const char* buf, const int size);
    // TODO Gjør følgande til peikarar, slik at eg ikkje treng å include asio!
    boost::asio::io_service ioService;
    boost::asio::serial_port serialPort;
    const std::string serialPortPath;
};

#endif
