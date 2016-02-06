#define BOOST_TEST_MODULE serial_communication
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <cstdlib>

#include "serialInterface.h"

using std::cout;
//namespace utf = boost::unit_test;
namespace bASIO = boost::asio;

// The following serial ports are crated with [socat] shell command, and is
// used for dummy-testing in TDD.
static const std::string PATH_VIRTUAL_SERIAL_PORT_INPUT = "/dev/pts/9";
static const std::string PATH_VIRTUAL_SERIAL_PORT_OUTPUT= "/dev/pts/11";


BOOST_AUTO_TEST_SUITE(serial_communication_through_virtual_serial_port);
bool fileExists(const std::string& filePath)
{
    if (FILE *file = fopen(filePath.c_str(), "r")) {
        fclose(file);
        return true;
    } else {
        return false; 
    }
}
BOOST_AUTO_TEST_CASE( SerialPortsExcists )
{
    BOOST_REQUIRE( fileExists(PATH_VIRTUAL_SERIAL_PORT_INPUT) ); 
    BOOST_REQUIRE( fileExists(PATH_VIRTUAL_SERIAL_PORT_OUTPUT) ); 
}
BOOST_AUTO_TEST_CASE( ConstructSerialObject )
{
    boost::system::error_code ec;
    Serial testObj(PATH_VIRTUAL_SERIAL_PORT_OUTPUT, 9600, &ec);

    // Bare test om objektet eksisterer: check ec for feil..
    BOOST_CHECK_MESSAGE( !ec, "Construction of serial object gave error message" <<ec.message().c_str() );
}
void writeStringToFilepath(const std::string& argString, const std::string& argPath)
{
    std::string cmdString = "echo '" + argString + "' > " + argPath;
    system(cmdString.c_str());
}
BOOST_AUTO_TEST_CASE( serial_read )
{
    Serial receivePort(PATH_VIRTUAL_SERIAL_PORT_OUTPUT); 
    std::string testString = "asdfqwer1234æøå";
    std::string readBuffer;

    writeStringToFilepath(testString, PATH_VIRTUAL_SERIAL_PORT_INPUT);
    receivePort.read(&readBuffer);
    BOOST_CHECK_EQUAL(readBuffer, testString);   
}
std::string exec(const char* cmd) {
    std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
    if (!pipe) return "ERROR";
    char buffer[128];
    std::string result = "";
    while (!feof(pipe.get())) {
        if (fgets(buffer, 128, pipe.get()) != NULL) {
            result += buffer;
        }
    }
    // remove last char (this is always(?) an extra '\n' from BASH cmd return).
    result.erase(result.size()-1);
    return result;
}
BOOST_AUTO_TEST_CASE( serial_write )
{
    Serial sendPort(PATH_VIRTUAL_SERIAL_PORT_INPUT);
    std::string testString = "asdfqwer1234";
    std::string cmdString = "head -1 " + PATH_VIRTUAL_SERIAL_PORT_OUTPUT;
        // Command reads last line in path (without '\n' on end)
    sendPort.write_message(testString);
    usleep(1000);

    std::string returnFromShell = exec(cmdString.c_str());

    BOOST_CHECK_EQUAL(returnFromShell, testString);
}

BOOST_AUTO_TEST_CASE( send_message_through_virtual_serial_port )
{
    Serial sendPort(PATH_VIRTUAL_SERIAL_PORT_INPUT);
    Serial receivePort(PATH_VIRTUAL_SERIAL_PORT_OUTPUT);

    std::string testString = "asdfqwer1234æøå";
    std::string stringRead = "";
    sendPort.write_message(testString);
    receivePort.read(&stringRead);
    BOOST_CHECK_EQUAL(testString, stringRead);
}

#if 0
BOOST_TEST_CASE( ReadFromArduino )
{
    // MERK: dersom man sender ' ' (space) inn i arduino-monitoren, får man
    // ikkje med dei to første characters. Dette gir ønske om å teste for
    // start-character også (i read-funksjonen)!
    std::string path = "/dev/ttyACM0"; // Eller noke i den duren
    BOOST_REQUIRE( fileExists(path) );
    Serial testObj(path, 9600);
    std::string lestStreng;
    BOOST_CHECK( testObj.read(&lestStreng) );   // returnerer false ved feil.
    std::cout<<"HURRA: Eg har lest : [[[" <<lestStreng <<"]]]\n";
}
#endif
BOOST_AUTO_TEST_SUITE_END(); // serial_communication_with_tempfile



/*
BOOST_AUTO_TEST_SUITE( Arduino_serial_communication );
BOOST_AUTO_TEST_CASE( send_and_receive )
{
    Serial realArduino("/dev/ttyASM1");
    realArduino.sendMessage("fraSoftware");
}
BOOST_AUTO_TEST_SUITE_END();
*/
// TODO hugs å finne/respondere for exception ved feil filepath! TODO Ved
// construction av Serial object!
// TODO
