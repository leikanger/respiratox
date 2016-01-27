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
BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(serial_communication_with_tempfile);
BOOST_AUTO_TEST_CASE( writeToTempfile )
{
    system("touch ./tempFile.log");
    Serial serialInterface("./tempFile.log");
    std::string sendtStreng = "ASDFqwer1234";
    serialInterface.sendMessage(sendtStreng);
    usleep(100);
    std::ifstream fileInput("./tempFile.log");
    std::string lestTekststreng;
    std::getline(fileInput, lestTekststreng);
    BOOST_CHECK_EQUAL(sendtStreng, lestTekststreng);
    //system("rm ./tempFile.log");
}
BOOST_AUTO_TEST_CASE( writeToLiveDevFile )
{
    Serial liveTest("/dev/ttyACM1");
    liveTest.sendMessage("LIVE");
}
BOOST_AUTO_TEST_SUITE_END();



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
