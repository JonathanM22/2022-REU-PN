/*
    Made By Jonathan Mathews V1.1
    Libaries used are made from other people

    This code Uses the ADXL357z breakout board is used
    with a SD card module and a ArSduino Mega 256. The code
    gets data from the ADXL357 and writes to a micro SD for
    post analysis

    --Version Tests
    1) Wire.setClock(3400000); -> 1571
    2) Wire.setClock(3400000); -> 1579
    3) No Wire Set -> 3318

*/

//  - - - - - - - - - - - - - ADXL - - - - - - - - - - - - - - - - - - - //
#include "Seeed_adxl357b.h"


#if defined(ARDUINO_ARCH_AVR)
#pragma message("Defined architecture for ARDUINO_ARCH_AVR.")
#define SERIAL Serial
#elif defined(ARDUINO_ARCH_SAM)
#pragma message("Defined architecture for ARDUINO_ARCH_SAM.")
#define SERIAL SerialUSB
#elif defined(ARDUINO_ARCH_SAMD)
#pragma message("Defined architecture for ARDUINO_ARCH_SAMD.")
#define SERIAL SerialUSB
#elif defined(ARDUINO_ARCH_STM32F4)
#pragma message("Defined architecture for ARDUINO_ARCH_STM32F4.")
#define SERIAL SerialUSB
#else
#pragma message("Not found any architecture.")
#define SERIAL Serial
#endif


#define CALI_BUF_LEN           15
#define CALI_INTERVAL_TIME     250
int32_t cali_buf[3][CALI_BUF_LEN];
int32_t cali_data[3];

float factory;

//ADXL Object
Adxl357b  adxl357b;

//Data Vars
int32_t x, y, z;
uint8_t entry = 0;

//Calibration Buffer Thing - - - - - - - - - - -
int32_t deal_cali_buf(int32_t* buf) {
  int32_t cali_val = 0;

  for (int i = 0; i < CALI_BUF_LEN; i++) {
    cali_val += buf[i];
  }
  cali_val = cali_val / CALI_BUF_LEN;
  return (int32_t)cali_val;
}

// Cailbration Function - - - - - - - - - - - -
void calibration(void) {
  int32_t x;
  SERIAL.println("Please Place the module horizontally!");
  delay(1000);
  SERIAL.println("Start calibration........");

  for (int i = 0; i < CALI_BUF_LEN; i++) {
    if (adxl357b.checkDataReady()) {
      if (adxl357b.readXYZAxisResultData(cali_buf[0][i], cali_buf[1][i], cali_buf[2][i])) {
      }
    }
    delay(CALI_INTERVAL_TIME);
    // SERIAL.print('.');
  }
  // SERIAL.println('.');
  for (int i = 0; i < 3; i++) {
    cali_data[i] =  deal_cali_buf(cali_buf[i]);
    SERIAL.println(cali_data[i]);
  }
  x = (((cali_data[2] - cali_data[0]) + (cali_data[2] - cali_data[1])) / 2);
  factory = 1.0 / (float)x;
  // SERIAL.println(x);
  SERIAL.println("Calibration OK!!");
}

//  - - - - - - - - - - - - - SD - - - - - - - - - - - - - - - - - - - //

#include <SD.h>
#include <SPI.h>
const int chipSelect = 4;
File myFile;
File testFile;

String fileName = "log6_2.txt";

//  - - - - - - - - - - - - - OTHER - - - - - - - - - - - - - - - - - - - //
unsigned long currentMillis;
unsigned long startMillis;
const unsigned long period = 1000 * 10;
unsigned long deltaTime;
int counter = 0;
int countflag = 0;



//  - - - - - - - - - - - - - VOID SETUP - - - - - - - - - - - - - - - - - - - //

void setup(void) {
  //Begin Serial Mointor
  SERIAL.begin(115200);

  //Print file name for user to see
  Serial.println("- - - - ");
  Serial.print(fileName);
  Serial.print("- - - - ");

  
  // - - SD CARD SETUP - - //
  initialSD();

  // - - ADXL SETUP - - //
  uint8_t value = 0;
  float t;

  if (adxl357b.begin()) {
    SERIAL.println("Can't detect ADXL357B device .");
    while (1);
  }
  SERIAL.println("Init OK!");
  /*Set full scale range to ±20g*/
  adxl357b.setAdxlRange(TWENTY_G);
  /*Switch standby mode to measurement mode.*/
  adxl357b.setPowerCtr(0);
  delay(100);
  /*Read Uncalibration temperature.*/
  adxl357b.readTemperature(t);

  SERIAL.print("Uncalibration  temp = ");
  SERIAL.println(t);
  /**/
  calibration();

  //Printing out delay to give user some time to set up measurement
  Serial.print("TESTING IN 5 SECONDS . . .");
  for (int x = 1; x <= 5; x++) {
    Serial.print(" ");
    Serial.print(x);
    Serial.print(". . . ");
    delay(1000);
  }
  Serial.println("Testing Began. . .");


}

//  - - - - - - - - - - - - - VOID LOOP - - - - - - - - - - - - - - - - - - - //
void loop(void) {

  startMillis = millis();

  while (deltaTime <= period) {
    //Timer
    currentMillis = millis();
    deltaTime = currentMillis - startMillis;

    //Retreving & Writing Data
    getData();
    //printData();
    writeSD();
  }

  //Counter
  if (countflag == 0) {
    Serial.print("Test Done:   ");
    myFile.close();                  //Close File
    Serial.print("Closed, ");
    Serial.println(counter);
  }
  countflag = 1;

}
//  - - - - - - - - - - - - - VOID LOOP END - - - - - - - - - - - - - - - - - - - //

//Get DATA - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void getData() {

  if (adxl357b.checkDataReady()) {
    if (adxl357b.readXYZAxisResultData(x, y, z)) {
      SERIAL.println("Get data failed!");
    }
    /*
      SERIAL.print("X axis = ");
      SERIAL.print(x * factory);
      SERIAL.println('g');
      SERIAL.print("Y axis = ");
      SERIAL.print(y * factory);
      SERIAL.println('g');
      SERIAL.print("Z axis = ");
      SERIAL.print(z * factory);
      SERIAL.println('g');
    */
  }

  //counter++;

}

//Write To SD - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void writeSD() {

  myFile.print(millis()); myFile.print(",");
  myFile.print(x); myFile.print(",");
  myFile.print(y); myFile.print(",");
  myFile.println(z);

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
  testFile = SD.open("test21.txt", FILE_WRITE);
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

//PRINT DATA - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void printData() {
  SERIAL.print("X axis = ");
  SERIAL.print(x * factory);
  SERIAL.println('g');
  SERIAL.print("Y axis = ");
  SERIAL.print(y * factory);
  SERIAL.println('g');
  SERIAL.print("Z axis = ");
  SERIAL.print(z * factory);
  SERIAL.println('g');
}
