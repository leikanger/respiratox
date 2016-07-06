#ifndef SERIALINTERFACETEST_H
#define SERIALINTERFACETEST_H

#define BOOST_TEST_MODULE serial_communication
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <cstdlib>

#include <thread>
#include <chrono>

// The two paths are extracted to the following includefile, for convenience..
#include "pathToVirtualSerialPorts.h"
#include "serialBOOST.h"

namespace TEST{
    struct ArduinoMOCK {
        SerialBOOST serialPort; 
        bool bContinueExecution;
        std::thread mThread;
        const std::string constMessage;
        const unsigned int delayBetweenMessagesMs;
    
        ArduinoMOCK(std::string argConstMessage ="", unsigned int argDelayBetweenMessagesMs =200) 
          : serialPort(PATH_VIRTUAL_SERIAL_PORT_SEND) 
          , bContinueExecution{true}
          , constMessage(argConstMessage)
          , delayBetweenMessagesMs(argDelayBetweenMessagesMs)
          //, mThread([](ArduinoMOCK* pThis){ std::cout<<"lambda bool: " <<pThis->bContinueExecution <<"\n"; pThis->run(); } , this)
        {
            // Todo: move into initializer list?
            mThread = std::thread([](ArduinoMOCK* ardMOCKobject) { ardMOCKobject->run(); }, this ) ;
        }
        ~ArduinoMOCK() {        stop();         }
    
        void stop()
        {
            if (bContinueExecution) {
                bContinueExecution = false;
                mThread.join();
            }
        }
        void run()
        {
            std::string nextMessage = "INIT MESSAGE\n";
            std::cout<<"Messages sent: ";
            while(bContinueExecution) 
            {
                nextMessage=getNextMessage();
                using ms = std::chrono::milliseconds;
                std::this_thread::sleep_for(ms(delayBetweenMessagesMs));
    
                // progress bar - show dots for each msg
                std::cerr<<".";
                serialPort.write_message(nextMessage);
            }
            std::cout<<std::endl;
        }
        // XXX The following method shows how messages shall be written
        std::string getNextMessage()
        {
            if (constMessage == "") {
                return "123.4\t444.44\t13241414.6";
            } else {
                return constMessage;
            }
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

    /// @brief Different ways a serial port may be flushed.
    enum flush_type
    {
        flush_receive = TCIFLUSH,
        flush_send = TCIOFLUSH,
        flush_both = TCIOFLUSH
    };

    /// @brief Flush a serial port's buffers.
    ///
    /// @param serial_port Port to flush.
    /// @param what Determines the buffers to flush.
    /// @param error Set to indicate what error occurred, if any.
    void flush_serial_port(
        boost::asio::serial_port& serial_port,
        flush_type what,
        boost::system::error_code& error)
    {
        if (0 == ::tcflush(serial_port.lowest_layer().native_handle(), what))
        {
            error = boost::system::error_code();
        } else {
            error = boost::system::error_code(errno,
                boost::asio::error::get_system_category());
        }
    }

    void emptyVirtualSerialportBuffers()
    {
        boost::asio::io_service ioservice;
        boost::asio::serial_port serialPort(ioservice, PATH_VIRTUAL_SERIAL_PORT_RECEIVE);
        boost::system::error_code ec;
        TEST::flush_serial_port(serialPort, flush_receive, ec);
        if (ec)
            std::cout<<ec.message() <<std::endl;
    }
    
} //end: namespace TEST
    
#endif
