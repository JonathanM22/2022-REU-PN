#include <Arduino.h>


#include "SparkFunLSM6DSO.h"
#include "Wire.h"
//#include "SPI.h"

LSM6DSO myIMU; //Default constructor is I2C, addr 0x6B


// - - - SD - - - //
#include "FS.h"
#include <SD.h>
#include <SPI.h>
const int chipSelect = 5;
File myFile;
File testFile;

String fileName = "/data01.txt";

#define chipSelect 5 
String data;

//Buffer
String dataBuffer = "";
unsigned int chunkSize;
const int bufferSize = 299;
int bufferpos = -1;
int trio = -1;

int16_t buffer[10];

int16_t ax, ay, az; //Vars to hold accell data


void setup() {
  Serial.begin(115200);
  delay(500);

  Wire.begin();
  delay(10);
  if ( myIMU.begin() )
    Serial.println("Ready.");
  else {
    Serial.println("Could not connect to IMU.");
    Serial.println("Freezing");
  }

  if (!SD.begin(chipSelect)) {
    Serial.print("initialization failed!");
    return;
  }

  //Testing - - - - - -

  if ( myIMU.initialize(BASIC_SETTINGS) ) {
  Serial.println("Loaded Settings.");

  Serial.print("Accel Data Rate: ");
  Serial.println(myIMU.getAccelDataRate());

  Serial.print("AccelHighPerf: ");
  Serial.println(myIMU.getAccelHighPerf());
  }

  ax = myIMU.readRawAccelX();
  buffer[0] = ax;

  int arrInt[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
  int  n = sizeof(arrInt) / sizeof(int);

  char *arrChar;
  arrChar = (char *) malloc (sizeof(char) * (n+1));
  int arrCharlen = sizeof(arrChar) / sizeof(*arrChar);

  char idk[5] = "aaaa";
  
  myFile = SD.open(fileName, FILE_WRITE);
  myFile.write(idk,4);
  mtFile.close(); 

}// STUP - - - - 






void loop()
{

  /*/Get all parameters
    Serial.print(" X = ");
    Serial.print(myIMU.readFloatAccelX(), 3);
    Serial.print(" Y = ");
    Serial.print(myIMU.readFloatAccelY(), 3);
    Serial.print(" Z = ");
    Serial.println(myIMU.readFloatAccelZ(), 3);*/

}
