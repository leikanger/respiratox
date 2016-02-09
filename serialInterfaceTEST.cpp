#define BOOST_TEST_MODULE serial_communication
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <cstdlib>

#include <thread>
#include <chrono>

#include "serialInterface.h"

// The two paths are extracted to the following includefile, for convenience..
#include "pathToVirtualSerialPorts.h"


using std::cout;
//namespace utf = boost::unit_test;
namespace bASIO = boost::asio;

struct ArduinoMOCK {
    Serial serialPort; 
    bool bContinueExecution;
    std::thread mThread;

    ArduinoMOCK() 
      : serialPort(PATH_VIRTUAL_SERIAL_PORT_INPUT) 
      , bContinueExecution{true}
      //, mThread([](ArduinoMOCK* pThis){ std::cout<<"lambda bool: " <<pThis->bContinueExecution <<"\n"; pThis->run(); } , this)
    {
        mThread = std::thread([](ArduinoMOCK* ardMOCKobject) { ardMOCKobject->run(); }, this ) ;
    }
    ~ArduinoMOCK()
    {
        bContinueExecution = false;
        mThread.join();
    }
    void run()
    {
        std::string nextMessage = "INIT MESSAGE\n";
        cout<<"Messages sent: ";
        while(bContinueExecution) 
        {
            nextMessage=getNextMessage();
            std::this_thread::sleep_for(std::chrono::milliseconds(200));

            // progress bar - show dots for each msg
            std::cerr<<".";
            serialPort.write_message(nextMessage);
        }
        cout<<std::endl;
    }
    // XXX The next funciton documents how messages shall be written
    std::string getNextMessage()
    {
        return "123.4\t444.44\t13241414.6";
    }
};


bool fileExists(const std::string& filePath)
{
    if (FILE *file = fopen(filePath.c_str(), "r")) {
        fclose(file);
        return true;
    } else {
        return false; 
    }
}
void writeStringToFilepath(const std::string& argString, const std::string& argPath)
{
    std::string cmdString = "echo '" + argString + "' > " + argPath;
    system(cmdString.c_str());
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

//==============================================================================================
//======================                TEST                  ==================================
//==============================================================================================
BOOST_AUTO_TEST_SUITE(serial_communication_through_virtual_serial_port);
BOOST_AUTO_TEST_CASE( serial_port_paths_excist )
{
    BOOST_REQUIRE( fileExists(PATH_VIRTUAL_SERIAL_PORT_INPUT) ); 
    BOOST_REQUIRE( fileExists(PATH_VIRTUAL_SERIAL_PORT_OUTPUT) ); 
}
BOOST_AUTO_TEST_CASE( construct_Serial_object )
{
    boost::system::error_code ec;
    Serial testObj(PATH_VIRTUAL_SERIAL_PORT_OUTPUT, 9600, &ec);

    // Bare test om objektet eksisterer: check ec for feil..
    BOOST_CHECK_MESSAGE( !ec, "Construction of serial object gave error message" <<ec.message().c_str() );
}
BOOST_AUTO_TEST_CASE( empty_buffer_when_construct_Serial_object )
{
    // Write an empty string to ports, to clear last line but also to certify
    // return from head -1 command..
    writeStringToFilepath("", PATH_VIRTUAL_SERIAL_PORT_INPUT);
    writeStringToFilepath("", PATH_VIRTUAL_SERIAL_PORT_OUTPUT);
    // Then get the output for the two paths (will get input from the other
    // path -- fails if the paths are set incorrectly)
    std::string cmdStringINPUT = "head -1 " + PATH_VIRTUAL_SERIAL_PORT_INPUT;
    std::string cmdStringOUPUT = "head -1 " + PATH_VIRTUAL_SERIAL_PORT_OUTPUT;
    std::string ret1 = exec(cmdStringINPUT.c_str());
    std::string ret2 = exec(cmdStringOUPUT.c_str());

    BOOST_CHECK_MESSAGE( ret1 == "", 
            "pipe __1__ was not clean at startup:\nPipe ONE was not clean at startup : please rerun test -- error if this text lingers");
    BOOST_CHECK_MESSAGE( ret2 == "",
            "pipe __2__ was not clean at startup:\nPipe TWO was not clean at startup : please rerun test -- error if this text lingers");
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
BOOST_AUTO_TEST_SUITE_END(); // serial_communication_with_tempfile

// NEXT: TESTING ArduinoMOCK class.. NOT WORKING TODO
BOOST_AUTO_TEST_SUITE( ArduinoMOCK_for_message_designing );
BOOST_AUTO_TEST_CASE( thread_test )
{
   using std::chrono::seconds;
   ArduinoMOCK test;
   std::this_thread::sleep_for(seconds(1));
   // Verify that all works as it should with ArduinoMOCK!
}
BOOST_AUTO_TEST_CASE( receive_messages_from_ArduinoMOCK )
{
    Serial receivePort(PATH_VIRTUAL_SERIAL_PORT_OUTPUT);
    ArduinoMOCK test;
    std::string message;
    std::cerr<<"\n";
    for( int i : {1,2,3,4,5} ) {
        receivePort.read(&message);
        cout<<i <<"'t message: " <<message <<std::endl;
    }
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
