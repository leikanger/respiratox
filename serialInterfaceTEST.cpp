#include "serialInterfaceTEST.h"
#include <memory>


using std::cout;
//namespace utf = boost::unit_test;
namespace bASIO = boost::asio;


/* TODO This next function is a free function!! TODO 
 * TODO     PLAN: Move into a new class!        TODO*/
#include <boost/lexical_cast.hpp>
std::vector<double> splitValueStringToValueVector(
/**/                /**/                     const std::string& valueString)
{
    using std::string;
    const char VALUE_SEPARATOR = '\t';

    const double DEFAULT_VAL = 0;
    std::vector<double> returnValues = {DEFAULT_VAL,DEFAULT_VAL,DEFAULT_VAL};
        // preinit vector so that we can return when error is detected

    string buffer = valueString;
    string::size_type nextMark = 0;
    for (int i = 0; i<3; ++i) {
        nextMark = buffer.find(VALUE_SEPARATOR);

        returnValues.at(i) =
                boost::lexical_cast<double>(buffer.substr(0,nextMark));
        buffer = buffer.substr(nextMark+1);
        // If nextMark is bigger than buffer.size, no more marks are found
        // Break loop and let possible remaining values default to DEFAULT_VAL.
        if (nextMark > buffer.size()) {
            break;
        }
    }
    return returnValues;
}
/* TODO PLAN: Move the previous function to appropriate class/place TODO */








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

    std::vector<double> readReceivedValueVector() {
        return splitValueStringToValueVector( this->pReceivePort->read() );
    }
    std::string receivedMessage() {
        return  pReceivePort->read();
    }
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
    SerialBOOST testObj(PATH_VIRTUAL_SERIAL_PORT_RECEIVE, 9600, &ec);

    // Bare test om objektet eksisterer: check ec for feil..
    BOOST_CHECK_MESSAGE( !ec, "Construction of serial object gave error: " 
            <<ec.message().c_str() );
}

BOOST_AUTO_TEST_CASE( empty_buffer_when_construct_Serial_object )
{
    TEST::emptyVirtualSerialportBuffers();

    TEST::writeStringToFilepath("", PATH_VIRTUAL_SERIAL_PORT_SEND);
    TEST::writeStringToFilepath("", PATH_VIRTUAL_SERIAL_PORT_RECEIVE);
    // Then get the output for the two paths (will get input from the other
    // path -- fails if the paths are set incorrectly)
    std::string cmdStringINPUT = "head -1 " + PATH_VIRTUAL_SERIAL_PORT_SEND;
    std::string cmdStringOUPUT = "head -1 " + PATH_VIRTUAL_SERIAL_PORT_RECEIVE;
    std::string ret1 = TEST::exec(cmdStringINPUT.c_str());
    std::string ret2 = TEST::exec(cmdStringOUPUT.c_str());

    BOOST_CHECK_MESSAGE( ret1 == "", 
            "pipe __1__ was not clean at startup");
    BOOST_CHECK_MESSAGE( ret2 == "",
            "pipe __2__ was not clean at startup");
}

BOOST_AUTO_TEST_CASE( serial_read )
{
    SerialBOOST receivePort(PATH_VIRTUAL_SERIAL_PORT_RECEIVE); 
    std::string testString = "asdfqwer1234æøå";

    TEST::writeStringToFilepath(testString, PATH_VIRTUAL_SERIAL_PORT_SEND);
    BOOST_CHECK_EQUAL(receivePort.read(), testString);
}

BOOST_AUTO_TEST_CASE( serial_write )
{
    SerialBOOST sendPort(PATH_VIRTUAL_SERIAL_PORT_SEND);
    std::string testString = "asdfqwer1234";
    // Read last line of shell output (without '\n' on end)
    std::string cmdString = "head -1 " + PATH_VIRTUAL_SERIAL_PORT_RECEIVE;
    sendPort.write_message(testString);
    usleep(100);

    std::string returnFromShell = TEST::exec(cmdString.c_str());

    BOOST_CHECK_EQUAL(returnFromShell, testString);
}

BOOST_AUTO_TEST_CASE( send_message_through_virtual_serial_port )
{
    SerialCommunicationFixture F;

    std::string testString = "asdfqwer1234æøå";
    F.pSendPort->write_message(testString);
    BOOST_CHECK_EQUAL(testString, F.receivedMessage());
}

BOOST_AUTO_TEST_CASE( separate_message_into_3_values )
{
    SerialCommunicationFixture F;

    F.pSendPort->write_message("111.11\t222\t3.3333");
    BOOST_CHECK_EQUAL(F.readReceivedValueVector().size(), 3);
}

BOOST_AUTO_TEST_CASE( separate_message_into_3_values_when_only_2_present )
{
    SerialCommunicationFixture F;

    // The message is separated into 3 value: DEFAULT_VAULE as third value..
    F.pSendPort->write_message("111.11\t222");
    BOOST_CHECK_EQUAL(F.readReceivedValueVector().size(), 3);
}

BOOST_AUTO_TEST_CASE( resulting_vector_from_message_splitting_seems_correct )
{
    SerialCommunicationFixture F;

    F.pSendPort->write_message("111.11\t222\t3.3333");
    std::vector<double> result = F.readReceivedValueVector();

    BOOST_CHECK_EQUAL(result[0], 111.11);
    BOOST_CHECK_EQUAL(result[1], 222);
    BOOST_CHECK_EQUAL(result[2], 3.3333);
}

BOOST_AUTO_TEST_CASE( badly_formed_data_does_not_give_error__3values )
{
    SerialCommunicationFixture F;

    F.pSendPort->write_message("111.\t.1\t.0");
    std::vector<double> result = F.readReceivedValueVector();

    BOOST_CHECK_EQUAL(result[0], 111);
    BOOST_CHECK_EQUAL(result[1], 0.1);
    BOOST_CHECK_EQUAL(result[2], 0);
}

BOOST_AUTO_TEST_CASE( large_data_also_gives_right_answer )
{
    SerialCommunicationFixture F;

    F.pSendPort->write_message("1000000000.\t-1000000000\t0");
    std::vector<double> result = F.readReceivedValueVector();

    BOOST_CHECK_EQUAL(result[0], 1000000000);
    BOOST_CHECK_EQUAL(result[1], -1000000000);
    BOOST_CHECK_EQUAL(result[2], 0);
}

BOOST_AUTO_TEST_CASE( no_third_value__no_separator )
{
    SerialCommunicationFixture F;

    F.pSendPort->write_message("111.1\t222.2");
    std::vector<double> result = F.readReceivedValueVector();
    
    BOOST_CHECK_EQUAL(result[0], 111.1);
    BOOST_CHECK_EQUAL(result[1], 222.2);
    // If last value doesn't exist, it shall be zero
    BOOST_CHECK_EQUAL(result[2], 0);
}

BOOST_AUTO_TEST_CASE( no_third_value__with_final_separator )
{
    SerialCommunicationFixture F;

    F.pSendPort->write_message("111.1\t222.2\t");
    std::vector<double> result = F.readReceivedValueVector();
    
    BOOST_CHECK_EQUAL(result[0], 111.1);
    BOOST_CHECK_EQUAL(result[1], 222.2);
    // If last value doesn't exist, it shall be zero (DEFAULT_VAULE)
    BOOST_CHECK_EQUAL(result[2], 0);
}

BOOST_AUTO_TEST_CASE( only_one_value_and_no_ending_separator )
{
    SerialCommunicationFixture F;

    F.pSendPort->write_message("111.1");
    std::vector<double> result = F.readReceivedValueVector();
    
    BOOST_CHECK_EQUAL(result[0], 111.1);
    // given '0' as DEFAULT_VAULE :
    BOOST_CHECK_EQUAL(result[1], 0);
    BOOST_CHECK_EQUAL(result[2], 0);
}

//*********************************
BOOST_AUTO_TEST_SUITE_END(); // serial_communication_with_tempfile

BOOST_AUTO_TEST_SUITE( ArduinoMOCK_for_message_designing );
// NEXT: TESTING ArduinoMOCK class.. NOT WELL TESTED TODO
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

    SerialBOOST receivePort(PATH_VIRTUAL_SERIAL_PORT_RECEIVE);
    std::string testMelding="Yeah, test message\t12\t34";
    TEST::ArduinoMOCK test(testMelding);
    for( int i=5; i>0; --i) {
        BOOST_CHECK_EQUAL(testMelding, receivePort.read());
    }
}

BOOST_AUTO_TEST_CASE( stress_test_receive_message_from_ArduinoMOCK )
{
    std::cout<<"Stress test: ";
    TEST::emptyVirtualSerialportBuffers();
    SerialBOOST receivePort(PATH_VIRTUAL_SERIAL_PORT_RECEIVE);
    std::string testMelding="Yeah, test message\t12\t34";
    TEST::ArduinoMOCK test(testMelding, 0);
    for( int i=0; i<100; ++i ) {
        BOOST_CHECK_EQUAL(testMelding, receivePort.read());
    }
    // This test is unverified: It has never failed..
}
BOOST_AUTO_TEST_SUITE_END(); // serial_communication_with_tempfile


