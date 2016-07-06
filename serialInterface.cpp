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
        //: serialMock(new SerialBOOST(portPath))
{
    std::cout<<"Constructor : Serial\n";
}

std::string Serial::read()
{
    return "UFERDIG";
}

int Serial::write_message(std::string pTextBuffer)
{
    return 0; // Denne skal ikkje returnere int! TODO
}
