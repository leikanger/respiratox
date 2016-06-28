#include "serialInterface.h"
#include <boost/date_time/posix_time/posix_time.hpp>
//#include <thread>

int main()
{
    static Serial comDev{"/dev/i2c-2"};

    boost::asio::io_service io;

    for (int i = 1000; i>0; --i) {
        boost::asio::deadline_timer timer(io, boost::posix_time::milliseconds(500));
        // Get new measurements from accelerometer here..
        timer.wait();

        comDev.write_message("jejeje\n");
    }
    return 0;
}
