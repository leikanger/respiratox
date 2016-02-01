#include "serialInterface.h"
#include <string>
#include <iostream>
#include <exception>

namespace bASIO=boost::asio;

// Serial interface through virtual serial port
/**
 * Constructor.
 * \param port device name, example "/dev/ttyUSB0" or "COM4"
 * \param baud_rate communication speed, example 9600 or 115200
 * \throws boost::system::system_error if cannot open the
 * serial device
**/
Serial::Serial(const std::string& portPath, unsigned int baudRateArg, boost::system::error_code* pec /*==null_ptr*/)
    : io(), serialPort(io)//serialPort(io, port)
{
    std::cout<<"Constructing Serial object with path: " <<portPath <<" baud_rate: " <<baudRateArg <<"\n";
    
    // For default argument *pec = 0 (no bs::error_code supplied), create
    // error code-dummy and take dummy's address:
    using BSerror_code = boost::system::error_code;
    BSerror_code dummyEC;
    if (pec == nullptr) { // erroc_code address supplied = 0 -- create local error-code obj.
        pec = &dummyEC;
    }
    serialPort.open(portPath, *pec);
    if (*pec) {
        std::cout<<"error : serialPort.open(" <<portPath <<",ec) reported error ec=" 
            <<pec->message().c_str() <<std::endl;
        // terminate!! 
        exit(0); // TODO return direkte herifra: Husk at ec--som-er-en-ref-variabel-- blir satt ved feil!! 
        // Kan sjekkes utafor!!!
    }

    serialPort.set_option( boost::asio::serial_port_base::baud_rate(baudRateArg), *pec);
    if (*pec) {
        std::cout<<"error : serialPort set baud_rate faila : reported error ec=" 
            <<pec->message().c_str() <<std::endl;
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
 *  An other variant is to save the rest of the message in a static variable,
 *  and continue where it left off last time -- and read in data in batches.
 *  Better for efficiency (but don't start with optimalization!)
 */
bool Serial::read(std::string* pTekstBuffer)
{
    const char MESSAGE_SEPARATOR = '\n';
    static char nextChar;
    *pTekstBuffer = "";

    boost::system::error_code ec;

    while (!ec) {
        bASIO::read(serialPort, bASIO::buffer(&nextChar, 1), ec);
        if (nextChar == MESSAGE_SEPARATOR)
            break;
        pTekstBuffer->push_back(nextChar);
    }
    if (ec) {
        std::cout<<"error : serialPort set baud_rate faila : reported error ec=" 
            <<ec.message().c_str() <<std::endl;
        // terminate!! 
        exit(0);
        return false;
    }

    if (nextChar != '\n') {
		// it must have timed out.
        return false;
		//throw std::exception("Read timed out!");
	}
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
