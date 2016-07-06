#include <string>
#include <iostream>
#include <boost/asio.hpp>
#include <fstream>
#include <vector>

#ifndef SERIALINTERFACE_H
#define SERIALINTERFACE_H

/************/
/** Serial **/
/************/
class Serial {
public:
    Serial(const std::string& port, unsigned int baud_rate =9600, boost::system::error_code* pec =nullptr);
    ~Serial();

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
