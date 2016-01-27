#define BOOST_TEST_MODULE serial_communication
#define BOOST_TEST_DYN_LINK
#include <boost/test/included/unit_test.hpp>

#include "serialInterface.h"

using std::cout;
namespace utf = boost::unit_test;

BOOST_AUTO_TEST_SUITE(serial_communication);
BOOST_AUTO_TEST_CASE( jeje )
{
    BOOST_CHECK_EQUAL( 9, 9 ); 
}

BOOST_AUTO_TEST_CASE( constructing )
{
    MockSerial serial;
    BOOST_CHECK_EQUAL( "", serial.getMessage() );
}

BOOST_AUTO_TEST_CASE( mockSendAndReceive )
{
    MockSerial serial;

    serial.sendMessage("melding");
    BOOST_CHECK_EQUAL("melding", serial.getMessage());
}

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
BOOST_AUTO_TEST_SUITE_END();
