/*

Testing Accell With SD Card With defult settings

*/

//Include Arduino
#include <Arduino.h>

// - - - Acell - - - //
#include "SparkFunLSM6DSO.h"
#include "Wire.h"

LSM6DSO myIMU;  //Default constructor is I2C, addr 0x6B

float ax,ay,az; //Vars to hold accell data

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

// - - - MY STUFF - - - //
unsigned long currentMillis;
unsigned long startMillis;
const unsigned long period = 1000 * 10;
unsigned long deltaTime;
int counter = 0;
int countflag = 0;

// - - - - - - - - - - - - - - - - - -  FUNCTIONS - - - - - - - - - - - - - - - - - - - //


//Get Data From Accel - - - -
void getData(){
ax = myIMU.readFloatAccelX();
ay = myIMU.readFloatAccelY();
az = myIMU.readFloatAccelZ();
}

//Write Data To The SD Card - - - -
void writeData(){

  ax = myIMU.readFloatAccelX();
  ay = myIMU.readFloatAccelY();
  az = myIMU.readFloatAccelZ();
  
  myFile.print(millis()); myFile.print(",");
  myFile.print(ax); myFile.print(",");
  myFile.print(ay); myFile.print(",");
  myFile.println(az);
  
  counter++;

}

//Intial SD - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void initialSD() {
  int header_check = 0;

  Serial.println("Initializing SD card...");
  pinMode(chipSelect, OUTPUT);
  if (!SD.begin(chipSelect)) {
    Serial.print("initialization failed!");
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
    delay(500);

    myFile = SD.open(fileName, FILE_WRITE);
    Serial.println(F(" Opened "));
    delay(500);
  }
}

//Print Data In Serial Monitor - - - -
void printData(){

  Serial.print("Accel XYZ: ");
  Serial.print(myIMU.readFloatAccelX(), 3); Serial.print(" ");
  Serial.print(myIMU.readFloatAccelY(), 3); Serial.print(" ");
  Serial.print(myIMU.readFloatAccelZ(), 3); Serial.print(" ");

  Serial.print("| Gyro XYZ: ");
  Serial.print(myIMU.readFloatGyroX(), 3); Serial.print(" ");
  Serial.print(myIMU.readFloatGyroY(), 3); Serial.print(" ");
  Serial.print(myIMU.readFloatGyroZ(), 3); Serial.print(" ");

  Serial.print("Temp F: ");
  Serial.println(myIMU.readTempF(), 3);
  counter++;
  
}

// - - - - - - - - - - - - - - - - - -  SETUP - - - - - - - - - - - - - - - - - - - //
void setup() {

  //Serial Setup
  Serial.begin(115200);
  delay(500); 
  
    //Accel Settings
  myIMU.setGyroDataRate(0);
  myIMU.setAccelDataRate(1660);

  //Acell Setup
  Wire.begin();
  delay(10);
  if( myIMU.begin() )
    Serial.println("Ready.");
  else { 
    Serial.println("Could not connect to IMU.");
    Serial.println("Freezing");
  }

  //SD Setup
  initialSD();



  Serial.println("Loaded Acell Settings.");

  delay(500); 

 //monitor_port = COM[11]
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
