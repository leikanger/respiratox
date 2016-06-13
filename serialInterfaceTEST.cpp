#include "serialInterfaceTEST.h"
#include <memory>


using std::cout;
//namespace utf = boost::unit_test;
namespace bASIO = boost::asio;

// Fixture
struct SerialCommunicationFixture {
    SerialCommunicationFixture()
        : pSendPort(new Serial(PATH_VIRTUAL_SERIAL_PORT_SEND)),
        pReceivePort(new Serial(PATH_VIRTUAL_SERIAL_PORT_RECEIVE))
    {
        TEST::emptyVirtualSerialportBuffers();
    }
    ~SerialCommunicationFixture() { }
    
    std::shared_ptr<Serial> pSendPort;
    std::shared_ptr<Serial> pReceivePort;
};

//==============================================================================================
//======================                TEST                  ==================================
//==============================================================================================
BOOST_AUTO_TEST_SUITE(serial_communication_through_virtual_serial_port);
BOOST_AUTO_TEST_CASE( serial_port_paths_exist )
{
    BOOST_REQUIRE( TEST::fileExists(PATH_VIRTUAL_SERIAL_PORT_SEND) ); 
    BOOST_REQUIRE( TEST::fileExists(PATH_VIRTUAL_SERIAL_PORT_RECEIVE) ); 
}
BOOST_AUTO_TEST_CASE( construct_Serial_object )
{
    boost::system::error_code ec;
    Serial testObj(PATH_VIRTUAL_SERIAL_PORT_RECEIVE, 9600, &ec);

    // Bare test om objektet eksisterer: check ec for feil..
    BOOST_CHECK_MESSAGE( !ec, "Construction of serial object gave error message" <<ec.message().c_str() );
}
BOOST_AUTO_TEST_CASE( empty_buffer_when_construct_Serial_object )
{
    TEST::emptyVirtualSerialportBuffers();
        // Now, the rest of this test should  be redundant..

    // Write an empty string to ports, to clear last line but also to certify
    // return from head -1 command..
    TEST::writeStringToFilepath("", PATH_VIRTUAL_SERIAL_PORT_SEND);
    TEST::writeStringToFilepath("", PATH_VIRTUAL_SERIAL_PORT_RECEIVE);
    // Then get the output for the two paths (will get input from the other
    // path -- fails if the paths are set incorrectly)
    std::string cmdStringINPUT = "head -1 " + PATH_VIRTUAL_SERIAL_PORT_SEND;
    std::string cmdStringOUPUT = "head -1 " + PATH_VIRTUAL_SERIAL_PORT_RECEIVE;
    std::string ret1 = TEST::exec(cmdStringINPUT.c_str());
    std::string ret2 = TEST::exec(cmdStringOUPUT.c_str());

    BOOST_CHECK_MESSAGE( ret1 == "", 
            "pipe __1__ was not clean at startup:\nPipe ONE was not clean at startup : please rerun test -- error if this text lingers");
    BOOST_CHECK_MESSAGE( ret2 == "",
            "pipe __2__ was not clean at startup:\nPipe TWO was not clean at startup : please rerun test -- error if this text lingers");
}
BOOST_AUTO_TEST_CASE( serial_read )
{
    Serial receivePort(PATH_VIRTUAL_SERIAL_PORT_RECEIVE); 
    std::string testString = "asdfqwer1234æøå";
    std::string readBuffer;

    TEST::writeStringToFilepath(testString, PATH_VIRTUAL_SERIAL_PORT_SEND);
    receivePort.read(&readBuffer);
    BOOST_CHECK_EQUAL(readBuffer, testString);   
}
BOOST_AUTO_TEST_CASE( serial_write )
{
    Serial sendPort(PATH_VIRTUAL_SERIAL_PORT_SEND);
    std::string testString = "asdfqwer1234";
    std::string cmdString = "head -1 " + PATH_VIRTUAL_SERIAL_PORT_RECEIVE;
        // Command reads last line in path (without '\n' on end)
    sendPort.write_message(testString);
    usleep(1000);

    std::string returnFromShell = TEST::exec(cmdString.c_str());

    BOOST_CHECK_EQUAL(returnFromShell, testString);
}

BOOST_AUTO_TEST_CASE( send_message_through_virtual_serial_port )
{
    SerialCommunicationFixture F;

    std::string testString = "asdfqwer1234æøå";
    std::string stringRead = "";
    F.pSendPort->write_message(testString);
    F.pReceivePort->read(&stringRead);
    BOOST_CHECK_EQUAL(testString, stringRead);
}
BOOST_AUTO_TEST_CASE( separate_message_into_3_values )
{
    SerialCommunicationFixture F;

    F.pSendPort->write_message("111.11\t222\t3.3333");
    std::vector<double> result = F.pReceivePort->getNextValueVector();
    BOOST_CHECK_EQUAL(result.size(), 3);
}
BOOST_AUTO_TEST_CASE( separate_message_into_3_values_when_only_2_present )
{
    // The message is separated into 3 value: DEFAULT_VAULE as third value..
    SerialCommunicationFixture F;

    F.pSendPort->write_message("111.11\t222");
    std::vector<double> result = F.pReceivePort->getNextValueVector();
    BOOST_CHECK_EQUAL(result.size(), 3);
}
BOOST_AUTO_TEST_CASE( resulting_vector_from_message_splitting_seems_correct )
{
    SerialCommunicationFixture F;

    F.pSendPort->write_message("111.11\t222\t3.3333");
    std::vector<double> result = F.pReceivePort->getNextValueVector();

    BOOST_CHECK_EQUAL(result[0], 111.11);
    BOOST_CHECK_EQUAL(result[1], 222);
    BOOST_CHECK_EQUAL(result[2], 3.3333);
}
BOOST_AUTO_TEST_CASE( badly_formed_data_does_not_give_error__3values )
{
    SerialCommunicationFixture F;

    F.pSendPort->write_message("111.\t.1\t.0");
    std::vector<double> result = F.pReceivePort->getNextValueVector();

    BOOST_CHECK_EQUAL(result[0], 111);
    BOOST_CHECK_EQUAL(result[1], 0.1);
    BOOST_CHECK_EQUAL(result[2], 0);
}
BOOST_AUTO_TEST_CASE( large_data_also_gives_right_answer )
{
    SerialCommunicationFixture F;

    F.pSendPort->write_message("1000000000.\t-1000000000\t0");
    std::vector<double> result = F.pReceivePort->getNextValueVector();

    BOOST_CHECK_EQUAL(result[0], 1000000000);
    BOOST_CHECK_EQUAL(result[1], -1000000000);
}
BOOST_AUTO_TEST_CASE( last_value_doesnt_exist_no_separator )
{
    SerialCommunicationFixture F;

    F.pSendPort->write_message("111.1\t222.2");
    std::vector<double> result = F.pReceivePort->getNextValueVector();
    
    BOOST_CHECK_EQUAL(result[0], 111.1);
    BOOST_CHECK_EQUAL(result[1], 222.2);
    // If last value doesn't exist, it shall be zero
    BOOST_CHECK_EQUAL(result[2], 0);

    cout<<result[0] <<" "<<result[1] <<" "  <<result[2] <<" " <<"\n";
}
BOOST_AUTO_TEST_CASE( last_value_doesnt_exist_with_separator )
{
    SerialCommunicationFixture F;

    F.pSendPort->write_message("111.1\t222.2\t");
    std::vector<double> result = F.pReceivePort->getNextValueVector();
    
    BOOST_CHECK_EQUAL(result[0], 111.1);
    BOOST_CHECK_EQUAL(result[1], 222.2);
    // If last value doesn't exist, it shall be zero
    BOOST_CHECK_EQUAL(result[2], 0);

    cout<<result[0] <<" "<<result[1] <<" "  <<result[2] <<" " <<"\n";
}

//*********************************
BOOST_AUTO_TEST_SUITE_END(); // serial_communication_with_tempfile

BOOST_AUTO_TEST_SUITE( ArduinoMOCK_for_message_designing );
// NEXT: TESTING ArduinoMOCK class.. NOT WORKING TODO
BOOST_AUTO_TEST_CASE( thread_test )
{
   using std::chrono::seconds;
   TEST::ArduinoMOCK test;
   std::this_thread::sleep_for(seconds(1));
   // Verify that all works as it should with ArduinoMOCK!
}
BOOST_AUTO_TEST_CASE( receive_messages_from_ArduinoMOCK )
{
    // Empty previous messages through virtual serial port
    TEST::emptyVirtualSerialportBuffers();

    Serial receivePort(PATH_VIRTUAL_SERIAL_PORT_RECEIVE);
    std::string testMelding="Yeah, test message\t12\t34";
    TEST::ArduinoMOCK test(testMelding);
    std::string message;
    for( int i=5; i>0; --i) {
        receivePort.read(&message);
        BOOST_CHECK_EQUAL(message, testMelding);
    }
}
BOOST_AUTO_TEST_CASE( stress_test_receive_message_from_ArduinoMOCK )
{
    std::cout<<"Stress test: ";
    TEST::emptyVirtualSerialportBuffers();
    Serial receivePort(PATH_VIRTUAL_SERIAL_PORT_RECEIVE);
    std::string testMelding="Yeah, test message\t12\t34";
    TEST::ArduinoMOCK test(testMelding, 0);
    std::string message;
    for( int i=0; i<100; ++i ) {
        receivePort.read(&message);
        BOOST_CHECK_EQUAL(message, testMelding);
    }
    // This test is unverified: It has never failed..
}
BOOST_AUTO_TEST_SUITE_END(); // serial_communication_with_tempfile



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
