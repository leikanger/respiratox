#include "ADXL345_I2C.h"

 ADXL345_I2C accelerometer(p28, p27);
 Serial pc(USBTX, USBRX);

 int main() {
       pc.baud(115200);
     int readings[3] = {0, 0, 0};
     
     pc.printf("Starting ADXL345 test...\n");
     wait(.001);
     pc.printf("Device ID is: 0x%02x\n", accelerometer.getDeviceID());
    wait(.001);
    
     // These are here to test whether any of the initialization fails. It will print the failure
    if (accelerometer.setPowerControl(0x00)){
         pc.printf("didn't intitialize power control\n"); 
         return 0;  }
     //Full resolution, +/-16g, 4mg/LSB.
     wait(.001);
     
     if(accelerometer.setDataFormatControl(0x0B)){
        pc.printf("didn't set data format\n");
        return 0;  }
     wait(.001);
     
     //3.2kHz data rate.
     if(accelerometer.setDataRate(ADXL345_3200HZ)){
        pc.printf("didn't set data rate\n");
        return 0;    }
     wait(.001);
     
     //Measurement mode.
     
     if(accelerometer.setPowerControl(MeasurementMode)) {
        pc.printf("didn't set the power control to measurement\n"); 
        return 0;   } 
 
     while (1) {
     
         wait(0.1);
         
         accelerometer.getOutput(readings);
         

        pc.printf("%i, %i, %i\n", (int16_t)readings[0], (int16_t)readings[1], (int16_t)readings[2]);
     }
 
 }
