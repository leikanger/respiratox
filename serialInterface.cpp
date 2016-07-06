#include "serialInterface.h"
#include <exception>



// Serial interface through virtual serial port
/**
 * Serial::Serial( const string& portPath, unsinged baudRate, errorCode)
 *      \param port device name, example "/dev/ttyUSB0" or "COM4"
 *      \param baud_rate communication speed, example 9600 or 115200
 *      \throws boost::system::system_error if cannot open the
 *      serial device
**/
Serial::Serial( const std::string& portPath,
                unsigned int baudRateArg /*=9600*/,
                boost::system::error_code* pec /*==nullptr*/)
        : serialMock(new SerialBOOST(portPath, baudRateArg, pec))
{
    //    Her er veldig bra! Vi kan new'e inn andre subklasser av
    //    SerialAbstractClass inn i serialMock(...) når vi kjem så langt!
}

std::string Serial::read()
{
    return serialMock->read();
}

void Serial::write_message(std::string pTextBuffer)
{
    serialMock->write_message(pTextBuffer);
}
