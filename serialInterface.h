#include <string>
#include <iostream>
#include <fstream>
#include <boost/asio.hpp>
#include <vector>

#ifndef SERIALINTERFACE_H
#define SERIALINTERFACE_H

class SerialInterface {
public:
    virtual void sendMessage(const std::string &sstringArg) =0;
    virtual const std::string getMessage() const                  =0;
};

/************/
/** Serial **/
/************/
class Serial {
public:
    Serial(const std::string& port, unsigned int baud_rate =9600, boost::system::error_code* pec =nullptr);
    ~Serial();

    int read(std::string* pTextBuffer);
    int write_message(std::string pTextBuffer);
    std::vector<double> getNextValueVector();
private:
    int write_some(const char* buf, const int size);
    boost::asio::io_service io;
    boost::asio::serial_port serialPort;
    const std::string serialPortPath;
};


#endif
