#include <string>
#include <iostream>
#include <boost/asio.hpp>
#include <fstream>
#include <vector>

#ifndef SERIALINTERFACE_H
#define SERIALINTERFACE_H

/************/
/** SerialBOOST **/
/************/
class SerialBOOST {
public:
    SerialBOOST(const std::string& port, 
    /**/        unsigned int baud_rate =9600,
    /**/        boost::system::error_code* pec =nullptr);
    ~SerialBOOST();

    int read(std::string* pTextBuffer);
    int write_message(std::string pTextBuffer);
    std::vector<double> getNextValueVector();
private:
    int write_some(const char* buf, const int size);
    // TODO Gjør følgande til peikarar, slik at eg ikkje treng å include asio!
    boost::asio::io_service ioService;
    boost::asio::serial_port serialPort;
    const std::string serialPortPath;
};


#endif
