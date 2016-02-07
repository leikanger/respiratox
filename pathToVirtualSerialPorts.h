/*
 * This file is only for convenience: 
 *      with virtual serial ports, created by socat (or the script
 *      ./setUpVirtualSerialPort.sh), the virtual ports are created with an
 *      arbitratry path. This file is to make it more convenient to change
 *      this path.
 */

// The following serial ports are crated with [socat] shell command, and is
// used for dummy-testing in TDD.
static const std::string PATH_VIRTUAL_SERIAL_PORT_INPUT = "/dev/pts/3"; //9";
static const std::string PATH_VIRTUAL_SERIAL_PORT_OUTPUT= "/dev/pts/4";  //11";


