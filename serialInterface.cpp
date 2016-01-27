#include "serialInterface.h"
#include <iostream>

//#include <boost/asio.hpp>


// Serial
Serial::Serial(const std::string& filePathToSerialinterface )
{
    // TODO: Legg inn try {} catch {} feilhåndtering her!
    serialFileStream.open(filePathToSerialinterface.c_str(), std::fstream::in | std::fstream::out);
}
Serial::~Serial()
{
    serialFileStream.close(); 
}

void Serial::sendMessage(const std::string &sstringArg)
{
   serialFileStream<<sstringArg; 
}

const std::string Serial::getMessage() const
{
    std::string line;
    //std::getline(serialFileStream, line);
    return line;
}

// MOCK
void MockSerial::sendMessage(const std::string& sstringArg)
{
    lastString = sstringArg;
}
 
const std::string MockSerial::getMessage() const 
{
    return lastString;
}
