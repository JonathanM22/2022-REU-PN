/*

  Testing Accell With SD Card With defult settings
  Changed the code in libary
  2 seconds -> 974

*/

//Include Arduino
#include <Arduino.h>

// - - - Acell - - - //
#include "SparkFunLSM6DSO.h"
#include "Wire.h"

LSM6DSO myIMU;  //Default constructor is I2C, addr 0x6B

int16_t ax, ay, az; //Vars to hold accell data

// - - - SD - - - //
#include "FS.h"
#include "SD.h"
#include "SPI.h"
const int chipSelect = 5;
File myFile;
File testFile;

String fileName = "/data01.txt";

#define chipSelect 5
String data;

// - - - MY STUFF - - - //
unsigned long currentMillis;
unsigned long startMillis;
const unsigned long period = 1000 * 10;
unsigned long deltaTime;
int counter = 0;
int countflag = 0;

// - - - - - - - - - - - - - - - - - -  FUNCTIONS - - - - - - - - - - - - - - - - - - - //


//Get Data From Accel - - - -
void getData() {
  ax = myIMU.readRawAccelX();
  ay = myIMU.readRawAccelY();
  az = myIMU.readRawAccelZ();
  /*
    Serial.print("Accel XYZ: ");
    Serial.print(ax); Serial.print(" ");
    Serial.print(ay); Serial.print(" ");
    Serial.println(az);
    delay(1000);
  */
  counter++;
}

void writeData() {
  
  ax = myIMU.readFloatAccelX();
  ay = myIMU.readFloatAccelX();
  az = myIMU.readFloatAccelX();

  Serial.print("Accel XYZ: ");
  Serial.print(ax); Serial.print(" ");
  Serial.print(ay); Serial.print(" ");
  Serial.println(az);

  /*
    myFile.print(millis()); myFile.print(",");
    myFile.print(ax); myFile.print(",");
    myFile.print(ay); myFile.print(",");
    myFile.println(az);
  */
  counter++;

}

// - - - - - - - - - - - - - - - - - -  SETUP - - - - - - - - - - - - - - - - - - - //
void setup() {

  //Serial Setup - - -
  Serial.begin(115200);
  delay(500);

  //Accel Setup - - -
  Wire.begin();
  delay(100);
  if ( myIMU.begin() )
    Serial.println("Ready.");
  else {
    Serial.println("Could not connect to IMU.");
    Serial.println("Freezing");
  }

  if ( myIMU.initialize(BASIC_SETTINGS) ) {}
  Serial.println("Loaded Settings.");

  Serial.print("Accel Data Rate: ");
  Serial.println(myIMU.getAccelDataRate());
  delay(2000);

  //SD SetUp - - -
  initialSD();

  delay(1000);

}//SetUp


// - - - - - - - - - - - - - - - - - -  LOOP - - - - - - - - - - - - - - - - - - - //
void loop()
{
  startMillis = millis();

  while (deltaTime <= period) {
    //Timer
    currentMillis = millis();
    deltaTime = currentMillis - startMillis;

    //printData();
    writeData();
    //getData();
    //getDataBetter();
  }

  //Counter
  if (countflag == 0) {
    Serial.print("Test Done:   ");
    myFile.close();                  //Close File
    Serial.println("Closed File, ");
    Serial.println(counter);
  }
  countflag = 1;
}

//Intial SD - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void initialSD() {
  int header_check = 0;
  Serial.println("Initializing SD card...");
  pinMode(chipSelect, OUTPUT);
  if (!SD.begin()) {
    Serial.println("Card Mount Failed"); //Begin SD
    return;
  }
  uint8_t cardType = SD.cardType();
  if (cardType == CARD_NONE) {
    Serial.println("No SD card attached"); //Check If micro sd card slot is filled
    return;
  }
  Serial.println("initialization done.");
  testFile = SD.open("/test21.txt", FILE_WRITE);
  if (testFile) {
    Serial.print(F("Writing to test.txt..."));
    testFile.println(F("testing 1, 2, 3."));
    Serial.println(F("done. test"));
    testFile.close();
    header_check = 1;
  }
  else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }

  if (header_check == 1) {
    //Create Header For File
    myFile = SD.open(fileName, FILE_WRITE);
    myFile.println("Tm,Ax,Ay,Az");
    myFile.close();
    Serial.print("Header Created . . .");

    myFile = SD.open(fileName, FILE_APPEND);
    Serial.println(F(" Opened "));
  }
  delay(750);
}
