#include "serialInterface.h"
#include "pathToVirtualSerialPorts.h"
#include <boost/date_time/posix_time/posix_time.hpp>
//#include <thread>

int main()
{
    static Serial comDev("/dev/i2c-2");
    //static Serial comDev(PATH_VIRTUAL_SERIAL_PORT_SEND);

    boost::asio::io_service io;

    std::string textToBeSent = "";
    for (int i = 1000; i>0; --i) {
        boost::asio::deadline_timer timer(io, boost::posix_time::milliseconds(500));
        // Get new measurements from accelerometer here..
        timer.wait();

        textToBeSent = "jeje " + std::to_string(1000-i) + "\n";
        comDev.write_message(textToBeSent);
    }
    return 0;
}
