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

bool Serial::read(std::string* pTekstBuffer, const unsigned numberOfChars)
{
    std::cout<<"Forsøker å lese inn til buffer (med nå-innhold:" <<*pTekstBuffer 
             <<", og antall bokstaver: " <<numberOfChars <<std::endl;
    char charBuffer; //[numberOfChars+1] = '\0';
    boost::system::error_code ec;
    bASIO::read(serialPort, bASIO::buffer(&charBuffer, 1), ec);
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
