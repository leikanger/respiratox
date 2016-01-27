#include <string>
#include <iostream>
#include <fstream>

#ifndef SERIALINTERFACE_H
#define SERIALINTERFACE_H

class SerialInterface {
public:
    virtual void sendMessage(const std::string &sstringArg) =0;
    virtual const std::string getMessage() const                  =0;
};

/** Serial **/
class Serial : public SerialInterface {
    std::fstream serialFileStream;
public:
    Serial(const std::string& filePathToSerialinterface );
    ~Serial();

    void sendMessage(const std::string &sstringArg);
    const std::string getMessage() const;
};

/** mockSerial class **/
class MockSerial : public SerialInterface {
    std::string lastString;
public:
    MockSerial() : lastString("") {}

    void sendMessage(const std::string & sstringArg);
    const std::string getMessage() const;
};

#endif
