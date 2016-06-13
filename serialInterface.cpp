#include "serialInterface.h"
#include <string>
#include <iostream>
#include <exception>
#include <boost/lexical_cast.hpp>

namespace bASIO=boost::asio;

// Serial interface through virtual serial port
/**
 * Constructor.
 * \param port device name, example "/dev/ttyUSB0" or "COM4"
 * \param baud_rate communication speed, example 9600 or 115200
 * \throws boost::system::system_error if cannot open the
 * serial device
**/
Serial::Serial( const std::string& portPath,
                unsigned int baudRateArg /*=9600*/,
                boost::system::error_code* pec /*==null_ptr*/)
    : io(), serialPort(io), serialPortPath(portPath)
{
    //std::cout<<"Constructing Serial object with path: " <<portPath <<" baud_rate: " <<baudRateArg <<"\n";
    
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

std::vector<double> Serial::getNextValueVector()
{
    using std::string;
    const char VALUE_SEPARATOR = '\t';
    const double DEFAULT_VALUE = 0;
    // preinit vector so that we can return when error is detected
    // (setting the rest of the values to DEFAULT_VALUE)
    std::vector<double> returnValues = {    DEFAULT_VALUE,
                                            DEFAULT_VALUE,
                                            DEFAULT_VALUE};
    
    std::string buffer;
    read(&buffer);
    string::size_type nextMark = 0;
    for (int i = 0; i<3; ++i) {
        nextMark = buffer.find(VALUE_SEPARATOR);

        returnValues.at(i) =
                boost::lexical_cast<double>(buffer.substr(0,nextMark));
        buffer = buffer.substr(nextMark+1);
        // If nextMark is bigger than buffer.size, no more marks are found,
        // and if we have not found all 3 values, previous default
        // initialization to DEFAULT_VALUE makes it possible to break:
        if (nextMark > buffer.size() && i<3) {
            // but we have not found 3 values yet
            break;
        }
    }

    return returnValues;
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
int Serial::read(std::string* pTekstBuffer)
{
  /* Effektivisere, når alt funker:
   *    Alternativ 1: XXX Strategi: Les inn fleire tegn (type:20 chars), og 
   *    leit etter '\n' i denne sekvensen. Når det finnes, klargjør 
   *    returverdi med det som er før (sammen med en static buffer variabel),
   *    og lagre det som er etter '\n' i den samme buffervariabel (for neste
   *    melding).
   *
   *    Alternativ 2: XXX Statisk lengde på signalet: dette gjør at vi kan 
   *    lese ut denne lengden med asio.read -- det negative er at dette 
   *    kanskje går på bekostning av sikkerhet?
   */
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
        std::cout<<"error : serialPort set baud_rate faila : "
                 <<"reported error ec=" 
                 <<ec.message().c_str() <<std::endl;
        // terminate!! 
        exit(0);
        return -1;
    }

    if (nextChar != '\n') {
		// it must have timed out.
        return -2;
		//throw std::exception("Read timed out!");
	  }
    return 0;
}
/* important: messages are separated by the '\n' sign */
int Serial::write_message(std::string pTextBuffer)
{
    pTextBuffer += '\n';
    return write_some(pTextBuffer.c_str(), pTextBuffer.size());
}
int Serial::write_some(const char* buf, const int size)
{
   boost::system::error_code ec;
   if (!serialPort.is_open()) return -1;
   if (size == 0) return 0;

   return serialPort.write_some(boost::asio::buffer(buf, size), ec);
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
