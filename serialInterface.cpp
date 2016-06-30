#include "serialInterface.h"
#include <string>
#include <iostream>
#include <exception>
#include <boost/lexical_cast.hpp>

namespace bASIO=boost::asio;

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
                boost::system::error_code* pec /*==null_ptr*/)
    : ioService(), serialPort(ioService), serialPortPath(portPath)
{
    // For default argument *pec = 0 (no bs::error_code supplied), create
    // error code-dummy and take dummy's address:
    using std::cout;
    using BSerror_code = boost::system::error_code;

    BSerror_code dummyEC;
    if (pec == nullptr) { // erroc_code address supplied = 0 -- create local 
                          //    error-code obj.
        pec = &dummyEC;
    }
    serialPort.open(portPath, *pec);
    if (*pec) {
        std::cout<<"error : serialPort.open(" <<portPath 
            <<",ec) reported error ec=" <<pec->message().c_str() <<std::endl;
        // terminate!! 
        exit(0); // TODO return direkte herifra: Husk at ec--som-er-en-ref-variabel-- blir satt ved feil!! 
        // TODO  Throw exception.
    }

    serialPort.set_option( 
    /**/                   bASIO::serial_port_base::baud_rate(baudRateArg), 
    /**/                   *pec );
    serialPort.set_option( bASIO::serial_port_base::character_size(8) );
    serialPort.set_option( bASIO::serial_port_base::stop_bits(  /* ... */
    /**/                        bASIO::serial_port_base::stop_bits::one) );
	//port_->set_option(boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one));
	//port_->set_option(boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none));
	//port_->set_option(boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none));

    if (*pec) {
        cout<<"error : serialPort set baud_rate faila : reported error ec="
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

    const double DEFAULT_VAL = 0;
    std::vector<double> returnValues = {DEFAULT_VAL,DEFAULT_VAL,DEFAULT_VAL};
        // preinit vector so that we can return when error is detected
    
    std::string buffer;
    this->read(&buffer); 
    string::size_type nextMark = 0;
    for (int i = 0; i<3; ++i) {
        nextMark = buffer.find(VALUE_SEPARATOR);

        returnValues.at(i) =
                boost::lexical_cast<double>(buffer.substr(0,nextMark));
        buffer = buffer.substr(nextMark+1);
        // If nextMark is bigger than buffer.size, no more marks are found.
        // Break loop and let possible remaining values default to DEFAULT_VAL.
        if (nextMark > buffer.size()) {
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
int Serial::read(std::string* pTekstBuffer)
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
        std::cout<<"error : serialPort read(str*) failed : "
                 <<"reported error ec=" 
                 <<ec.message().c_str() <<std::endl;
        // terminate!! 
        exit(0);
        return -1;
        // IKKJE: throw std::string("Serial::read(str*) failed");
        // ( den er ikkje exception safe - kan kaste sjølv igjen.. )
    }

    if (nextChar != '\n') {
		// it must have timed out.
        return -2;
        // TODO Gå over til: : throw std::exception("Read timed out!");
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
