#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

int main(int argc, char** argv)
{								
    try {
        if (argc != 2) {
            std::cout <<"usage: client <host>" <<std::endl;
            return 1;
        }
        // All asio programs needs at least one io_service object:
        boost::asio::io_service io_service;
        // We need a name resolver object for turning arg[1] into an TCP endpoint
        tcp::resolver resolver(io_service);
        // A resolver takes a query object and turns it into a list of encpoint.
        tcp::resolver::query query(argv[1], "daytime");
            // For query creation: we use name of server (arvg)  XXX and the name of the service: "daytime"
        // The list of endpoints is returned
        tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
        // Create a socket from the enpoint
        tcp::socket socket(io_service);
        boost::asio::connect(socket, endpoint_iterator);
            // Vi har no kobla til innholdet i alle endpoint_iterator, fra 3 l. opp
        while (true) {
            boost::array<char, 128> buf;
            boost::system::error_code error;
            
            size_t len = socket.read_some(boost::asio::buffer(buf), error);

            // When the server closes the connection, it::tcp::socket::read_some() will exit with the
            // boost::asio::error::eof error, whick is how we know how to exit the loop
            if (error == boost::asio::error::eof)
                break;  //connection closed cleanly by peer
            else if (error)
                throw boost::system::system_error(error); // other error

            std::cout.write(buf.data(), len);
                // Skriver ut c-strings på cout måte..
            std::cout<<'\n';
        }



    } 
    catch (std::exception& e) {
        std::cerr <<e.what() <<std::endl;
    }
}
