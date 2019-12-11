
#include "Wire.h"
#include "I2Cdev.h"
#include "HMC5883L.h"

// Get the mag declination of your location here http://www.magnetic-declination.com/ and insert in # defined in next line
#define DECLINATION  -0.017 // In degrees
 
 HMC5883L mag;
  int16_t mx, my, mz;
//******************************************************************************
uint8_t Initialise_Compass() {
  bypass_MPU(); //obligatoir pour acceder a la boussole en i2C
  mag.initialize();
  
  if(!mag.testConnection()) {
    Debug.println("No HMC5883L compass found!");
    return 0;
  }

  #if defined Debug_All || defined Debug_Compass
    mag.getHeading(&mx, &my, &mz);
    Debug.println("----- Compass Found -----");
    Debug.print  ("mag:\t");
    Debug.print  (mx); Debug.print("\t");
    Debug.print  (my); Debug.print("\t");
    Debug.print  (mz); Debug.print("\t");
    Debug.println("--------------------------");
    Debug.println("");
 #endif

 return 1;
}

//******************************************************************************
float GetMagHeading() {
  
  // Read Magnetometer
  mag.getHeading(&mx, &my, &mz);
  
  float fHeading = RadToDeg(atan2(my, mx));  //All in degrees now
  
  fHeading += DECLINATION;  // Add magnetic declination

  fHeading = Normalise_360(fHeading);
  
  #if defined Debug_All || defined Debug_Compass
    /* Display the results (magnetic vector values are in micro-Tesla (uT)) */
 //   Debug.print("X: "); Debug.print(mx); Debug.print("  ");
 //   Debug.print("Y: "); Debug.print(my); Debug.print("  ");
 //   Debug.print("Z: "); Debug.print(mz); Debug.print("  ");Debug.println("uT");
    Debug.print("Heading = "); Debug.println(fHeading,0); 
  #endif   

  return fHeading;
}

//******************************************************************************

int16_t Normalise_360(int16_t arg) {
  if (arg < 0) arg += 360;
  if (arg > 359) arg -= 360;
  return arg;
}
//***************************************************
float RadToDeg (float _Rad) {
  return _Rad * 180 / PI;  
}
//***************************************************

void bypass_MPU() {
  //Bypass Mode
  Wire.beginTransmission(0x68);
  Wire.write(0x37);
  Wire.write(0x02);
  Wire.endTransmission();

  Wire.beginTransmission(0x68);
  Wire.write(0x6A);
  Wire.write(0x00);
  Wire.endTransmission();

  //Disable Sleep Mode
  Wire.beginTransmission(0x68);
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission();
}
