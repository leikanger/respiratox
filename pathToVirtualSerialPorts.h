/*
 * This file is only for convenience: 
 *      with virtual serial ports, created by socat (or the script
 *      ./setUpVirtualSerialPort.sh), the virtual ports are created with an
 *      arbitratry path. This file is to make it more convenient to change
 *      this path.
 *      XXX Read both the "input" and "output" paht from the return from
 *      setUpVirtualSerialPort.sh bash script - that needs to be run after
 *      each reboot or on new systems.
 */

// The following serial ports are crated with [socat] shell command, and is
// used for dummy-testing in TDD.
static const std::string PATH_VIRTUAL_SERIAL_PORT_SEND = "/dev/pts/5";
static const std::string PATH_VIRTUAL_SERIAL_PORT_RECEIVE= "/dev/pts/6";


