#include <ctime>
#include <iostream>
#include <string>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

/* Lager en funskjon som genererer teksten som sendes tilbake til client */
std::string make_daytime_string()
{
    using namespace std;
    time_t now = time(0);
    return ctime(&now);
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
