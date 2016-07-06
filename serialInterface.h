#include "serialBOOST.h"
#include <string>
#include <iostream>
#include <boost/asio.hpp>
#include <fstream>
#include <vector>

#ifndef SERIALINTERFACE_H
#define SERIALINTERFACE_H

/************/
/** Serial **/
/************/
class Serial {
    std::shared_ptr<SerialBOOST> serialMock;
public:
    Serial( const std::string& port,
    /**/    unsigned int baud_rate =9600, 
    /**/    boost::system::error_code* pec =nullptr);

    std::string read();
    void write_message(std::string pTextBuffer);
private:
};


#endif
