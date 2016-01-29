#include <string>
#include <iostream>
#include <fstream>
#include <boost/asio.hpp>

#ifndef SERIALINTERFACE_H
#define SERIALINTERFACE_H

class SerialInterface {
public:
    virtual void sendMessage(const std::string &sstringArg) =0;
    virtual const std::string getMessage() const                  =0;
};

/************/
/** Serial **/
/************/
class Serial {
public:
    Serial(const std::string& port, unsigned int baud_rate);
    ~Serial();

    bool read(std::string* pTextBuffer);
    //bool write(std::string* pTextBuffer, unsigned numberOfChars);
private:

    boost::asio::io_service io;
    boost::asio::serial_port serialPort;
};

/****************
 ** FileSerial **
 ***************/
class FileSerial : public SerialInterface {
    std::fstream serialFileStream;

public:
    FileSerial(const std::string& filePathToSerialinterface );
    ~FileSerial();

    void sendMessage(const std::string &sstringArg);
    const std::string getMessage() const;
};

/** mockSerial class **/
// Denne er utdatert. Kan takast bort (men da må man rydde opp i unit tests)
class MockSerial : public SerialInterface {
    std::string lastString;
public:
    MockSerial() : lastString("") {}

    void sendMessage(const std::string & sstringArg);
    const std::string getMessage() const;
};

#endif
