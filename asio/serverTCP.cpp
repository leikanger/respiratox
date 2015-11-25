#include <ctime>
#include <iostream>
#include <string>
#include <boost/asio.hpp>

#include <cstdint>      // for uint16_t
#include <cmath>        // for std::sin()

using boost::asio::ip::tcp;

/* Lager en funskjon som genererer teksten som sendes tilbake til client */
std::string make_daytime_string()
{
    using namespace std;
    time_t now = time(0);
    return ctime(&now);
}

/* PLAN:
 *  - fill up each of the three array [x,y,z] with values.
 *  - make a constant-sized array of uint16, with [x,y,z] vectors (serial).
 *      Like: x=[1,1,1],y=[2,2,2],z=[3,3,3] ==> array = [1,1,1,2,2,2,3,3,3]
 *  - send this array over TCP/ip as a package.
 */
std::vector<uint16_t> make_coord_array(size_t vectorLength)
{
    std::vector<uint16_t> returnVector(vectorLength);

    // Plan: make a sinus value and sent this. Static int t_value to remember the current phase of the synthetic signal
    static int t_value = 0;
    for (int i = 0; i<vectorLength; ++i) {
        ++t_value;
        returnVector[i] = i;
    }
    return returnVector;
}

int main()
{
    try {
        // Kvart asio program treng (minst) eitt io_service objekt.
        boost::asio::io_service io_service;
        // A ip::tcp::acceptor object needs to be created TO LISTEN FOR NEW CONNECTIONS
        tcp::acceptor acceptor(io_service,  tcp::endpoint(tcp::v4(), 13));
            // It is initialized to listen on TCP port 13, for IP version 4.
        // This is an iterative server, which means that it will handle one connection at a time.
        while (true) {
            // Create a socket that will represent the connection to the client, and wait for connection.
            tcp::socket socket(io_service);
            acceptor.accept(socket);
                // wait for connection..

            // A client is accessing our service! Do work and send data to the client
            std::string message = make_daytime_string();

            boost::system::error_code ignored_error;
            boost::asio::write(socket, boost::asio::buffer(message), ignored_error);
            sleep(1);
            message = make_daytime_string();
            boost::asio::write(socket, boost::asio::buffer(message), ignored_error);
            sleep(1);
            message = make_daytime_string();
            boost::asio::write(socket, boost::asio::buffer(message), ignored_error);
            sleep(1);
            message = make_daytime_string();
            boost::asio::write(socket, boost::asio::buffer(message), ignored_error);
            sleep(1);
            message = make_daytime_string();
            boost::asio::write(socket, boost::asio::buffer(message), ignored_error);
            sleep(1);
            while (true) {
                message = make_daytime_string();
                boost::asio::write(socket, boost::asio::buffer(message), ignored_error);
                sleep(1);
            }
        }
    } 
    catch (std::exception &e) {
        std::cerr <<e.what() <<std::endl;
    }

    return 0;

}
