#include "serialInterface.h"
#include <iostream>

namespace bASIO=boost::asio;

// Serial interface through virtual serial port
/**
 * Constructor.
 * \param port device name, example "/dev/ttyUSB0" or "COM4"
 * \param baud_rate communication speed, example 9600 or 115200
 * \throws boost::system::system_error if cannot open the
 * serial device
**/
Serial::Serial(const std::string& portPath, unsigned int baudRateArg)
    : io(), serialPort(io)//serialPort(io, port)
{
    std::cout<<"Constructing Serial object with path: " <<portPath <<" baud_rate: " <<baudRateArg <<"\n";
    boost::system::error_code ec;
    serialPort.open(portPath, ec);
    if (ec) {
        std::cout<<"error : serialPort.open(" <<portPath <<",ec) reported error ec=" 
            <<ec.message().c_str() <<std::endl;
        // terminate!! 
        exit(0);
    }

    serialPort.set_option( boost::asio::serial_port_base::baud_rate(baudRateArg), ec);
    if (ec) {
        std::cout<<"error : serialPort set baud_rate faila : reported error ec=" 
            <<ec.message().c_str() <<std::endl;
        // terminate!! 
        exit(0);
    }
}
Serial::~Serial()
{
    serialPort.close(); 
}

/**********************************
 * Serial::read(sting&, unsigned) *
 *  - reads out string of chars untill MESSAGE_SEPARATOR character
 *      (defined on top of function)
 *  TODO Define start of message? Fault-tolerance!    
 */
bool Serial::read(std::string& tekstBuffer, const unsigned numberOfChars)
{
    const char MESSAGE_SEPARATOR = '\n';
    static char nextChar;
    tekstBuffer = "";

    std::cout<<"Forsøker å lese inn til buffer (med nå-innhold:" <<tekstBuffer 
             <<", og antall bokstaver: " <<numberOfChars <<std::endl;
    boost::system::error_code ec;

    while (!ec) {
        bASIO::read(serialPort, bASIO::buffer(&nextChar, 1), ec);
        if (nextChar == MESSAGE_SEPARATOR)
            break;
        tekstBuffer.push_back(nextChar);
    }
    if (ec) {
        std::cout<<"error : serialPort set baud_rate faila : reported error ec=" 
            <<ec.message().c_str() <<std::endl;
        // terminate!! 
        exit(0);
        return false;
    }

#if 0
    char charBuffer[128] = {'\0'};
    bASIO::read(serialPort, bASIO::buffer(charBuffer, numberOfChars), ec);
    tekstBuffer = std::string(charBuffer);
    //tekstBuffer = std::string(charBuffer.begin(), charBuffer.end());
    return true;
#endif
    return true;
}

/*
bool Serial::sendMessage(const std::string& tekst) const
{
    static std::array<const char*, 1024> staticBuffer;
    staticBuffer.front() = tekst.c_str();
    serialPort.write_some(bASIO::buffer(staticBuffer));
#if 0
    static std::string staticStringBuffer;
    staticStringBuffer = tekst;
    serialPort.write_some(bASIO::buffer(tekst), tekst.length());
#endif

    return true;
}
*/

// FileSerial
FileSerial::FileSerial(const std::string& filePathToSerialinterface )
{
    // TODO: Legg inn try {} catch {} feilhåndtering her!
    serialFileStream.open(filePathToSerialinterface.c_str(), std::fstream::in | std::fstream::out);
}
FileSerial::~FileSerial()
{
    serialFileStream.close(); 
}

void FileSerial::sendMessage(const std::string &sstringArg)
{
   serialFileStream<<sstringArg; 
}

const std::string FileSerial::getMessage() const
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
