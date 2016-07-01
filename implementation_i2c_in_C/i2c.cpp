// https://www.kernel.org/doc/Documentation/i2c/dev-interface
#include "i2c.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <stdlib.h>

#include <linux/i2c-dev.h>
    // from aptitude i2c-tools
#include <sys/ioctl.h>

static int openI2Cfile(int adapterNr);
static int specifyDevI2cAddress(int file, int addr);

int main()
{
    int file = openI2Cfile(2);
    
    if (!specifyDevI2cAddress(file,0x1d)) {
        /* ERROR HANDLING: check errno to see what went wrong */
        exit(1);
    }

    
    // TESTING THE CODE: ******* TODO Extract into function!
     __u8 reg = 0x10; /* Device register to access */
     __s32 res;
    char buf[10];

    /* Using SMBus commands */
    res = i2c_smbus_read_word_data(file, reg);
    if (res < 0) {
        /* ERROR HANDLING: i2c transaction failed */
    } else {
        /* res contains the read word */
        printf("Las ut %s\n");
    }

#if 0
    /* Using I2C Write, equivalent of 
     i2c_smbus_write_word_data(file, reg, 0x6543) */
  buf[0] = reg;
  buf[1] = 0x43;
  buf[2] = 0x65;
  if (write(file, buf, 3) != 3) {
    /* ERROR HANDLING: i2c transaction failed */
  }

  /* Using I2C Read, equivalent of i2c_smbus_read_byte(file) */
  if (read(file, buf, 1) != 1) {
    /* ERROR HANDLING: i2c transaction failed */
  } else {
    /* buf[0] contains the read byte */
  }
#endif
    return 0;
}

// int openI2Cfile(int adapterNr) 
//        arg: adapterNr =2; // i2c device - /dev/i2c-???
static int openI2Cfile(int adapterNr) 
{
    int file =0;
    char fileName[20];

    snprintf(fileName, 19, "/dev/i2c-%d", adapterNr);
    file = open(fileName, O_RDWR);
    if (file<0) {
        /* ERROR handling: check errno to see what went wrong */
        exit(1);
    }
    return file;
}

// We have opened the right adapter - need to specify address for i2c dev
static int specifyDevI2cAddress(int file, int i2cAddr)
{
    // TODO Legg inn feilcheck!! TODO på argmuneta!
    if (ioctl(file, I2C_SLAVE, i2cAddr)<0) {
        return 1;
    }
    return 0;
}
