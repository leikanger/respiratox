#define BOOST_TEST_MODULE serial_communication
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <cstdlib>

#include "serialInterface.h"

using std::cout;
namespace utf = boost::unit_test;

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
    std::string path = "/dev/ttyACM1"; //"/dev/pts/3";
    Serial testObj(path, 9600);
    cout<<"Fekk åpna path " <<path <<std::endl;
    std::string lestStreng;
    testObj.read(lestStreng, 10);
    std::cout<<"HURRA: Eg har lest : |||_" <<lestStreng <<"_|||\n";
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
