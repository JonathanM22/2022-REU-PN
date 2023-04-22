/*

  Testing Accell With SD Card With defult settings
  Changed the code in libary
  MemCopy Buffer
  
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

//"/testz05.bin"
String fileName = "/testz11.bin";

#define chipSelect 5
String data;

// - - - MY STUFF - - - //
unsigned long currentMillis;
unsigned long startMillis;
const unsigned long period = 1000 * 10;
unsigned long deltaTime;
int counter = 0;
int countflag = 0;


// - - - BUFFER - - - //
//String dataBuffer = "";
unsigned int chunkSize;
int16_t bufferInt[255];
byte bufferByte[510];

// - - - - - - - - - - - - - - - - - -  FUNCTIONS - - - - - - - - - - - - - - - - - - - //


void writeData() {
  int x = 0;
  while (x < 255) {
    //bufferInt[x++] = myIMU.readRawAccelX();
    //bufferInt[x++] = myIMU.readRawAccelY();
    bufferInt[x++] = myIMU.readRawAccelZ();
  }

  memcpy(&bufferByte, &bufferInt, 510);
  myFile.write(bufferByte, 510);

  counter++;
}

// - - - - - - - - - - - - - - - - - -  SETUP - - - - - - - - - - - - - - - - - - - //
void setup() {

  //Serial Setup - - -
  Serial.begin(115200);
  delay(500);

  //Accel Setup - - -
  Wire.begin();
  Wire.setClock(400000);
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

  //Printing out delay to give user some time to set up measurement
  
  Serial.print("TESTING IN 5 SECONDS . . .");
  for (int x = 1; x <= 5; x++) {
    Serial.print(" ");
    Serial.print(x);
    Serial.print(". . . ");
    delay(1000);
  }
  Serial.println("Testing Began. . .");
  
  
}//SetUp


// - - - - - - - - - - - - - - - - - -  LOOP - - - - - - - - - - - - - - - - - - - //
void loop()
{
  startMillis = millis();

  while (deltaTime <= period) {
    //Timer
    currentMillis = millis();
    deltaTime = currentMillis - startMillis;

    writeData();

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

    myFile = SD.open(fileName, FILE_WRITE );
    Serial.println(F(" Opened "));
  }
  delay(750);
}