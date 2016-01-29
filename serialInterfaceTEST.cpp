#define BOOST_TEST_MODULE serial_communication
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <cstdlib>

#include "serialInterface.h"

using std::cout;
//namespace utf = boost::unit_test;
namespace bASIO = boost::asio;

BOOST_AUTO_TEST_SUITE(MOCK_serial_communication);
BOOST_AUTO_TEST_CASE( constructing )
{
    MockSerial serial;
    BOOST_CHECK_EQUAL( "", serial.getMessage() );
}

BOOST_AUTO_TEST_CASE( mockSendAndReceive )
{
    MockSerial serialMock;
    serialMock.sendMessage("melding");
    BOOST_CHECK_EQUAL("melding", serialMock.getMessage());
}
BOOST_AUTO_TEST_SUITE_END(); //MOCK_serial_communication

BOOST_AUTO_TEST_SUITE(serial_communication_with_tempfile);
BOOST_AUTO_TEST_CASE( writeToTempfile )
{
    system("touch ./tempFile.log");
    FileSerial serialInterface("./tempFile.log");
    std::string sendtStreng = "ASDFqwer1234";
    serialInterface.sendMessage(sendtStreng);
    usleep(100);
    std::ifstream fileInput("./tempFile.log");
    std::string lestTekststreng;
    std::getline(fileInput, lestTekststreng);
    BOOST_CHECK_EQUAL(sendtStreng, lestTekststreng);
    //system("rm ./tempFile.log");
}
bool fileExists(const std::string& filePath)
{
    if (FILE *file = fopen(filePath.c_str(), "r")) {
        fclose(file);
        return true;
    } else {
        return false; 
    }
}
BOOST_AUTO_TEST_CASE( ConstructSerialObject )
{
    std::string path = "/dev/pts/0";
    Serial testObj(path, 9600);
    boost::system::error_code ec;

    // XXX En variant: forsøk å skrive (og gå ut fra at den gir feilmelding
    // dersom det ikkje går gjennom, eller obj ikkje er konstruert)
    // XXX En variant: å lese ut en option fra porten.
//    bASIO::get_option(boost::asio::serial_port_base::baud_rate(baudRateArg), ec);
    if (ec) { std::cout<<"TEST: get_option failed with message: " <<ec.message().c_str() <<std::endl; }
}
BOOST_AUTO_TEST_CASE( ReadFromArduino )
{
    // MERK: dersom man sender ' ' (space) inn i arduino-monitoren, får man
    // ikkje med dei to første characters. Dette gir ønske om å teste for
    // start-character også (i read-funksjonen)!
    std::string path = "/dev/pts/3";
    Serial testObj(path, 9600);
    cout<<"Fekk åpna path " <<path <<std::endl;
    std::string lestStreng;
    BOOST_CHECK( testObj.read(&lestStreng) );   // returnerer false ved feil.
    std::cout<<"HURRA: Eg har lest : [[[" <<lestStreng <<"]]]\n";
}
/*
 * BOOST_AUTO_TEST_CASE( VerifyVirtualSerialPortOperation )
{
    std::string filepathSerial1 = "/dev/pts/3";
    std::string filepathSerial2 = "/dev/pts/4";
    //BOOST_CHECK(fileExists(filepathSerial1) && fileExists(filepathSerial2));
    std::cout<<"ctor kalles med arg: " <<filepathSerial1 <<", 96000\n";
    Serial port1{filepathSerial1, 96000};
    //Serial port2{"./VirtualSerialPortOut", 96000};
     
}*/

BOOST_AUTO_TEST_CASE( writeToLiveDevFile )
{
    // TODO: neste setning har ingen tester, og er dårlig..
    FileSerial liveTest("/dev/ttyACM1");
    liveTest.sendMessage("LIVE");
}
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

/*
BOOST_AUTO_TEST_CASE( Serial_send_and_receive )
{
    Serial mockInterface("./slettTEST");
    mockInterface.sendMessage("asdf");
    Serial mockReadInterface("./slettTEST");
    BOOST_CHECK_EQUAL("asdf", mockReadInterface.getMessage());
}
*/
