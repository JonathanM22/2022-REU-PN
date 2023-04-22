#include <Wire.h>

#define LSM 0x6B
#define OUTX_L_A 0x28
#define OUTX_H_A 0x29
#define OUTY_L_A 0x2A
#define OUTY_H_A 0x2B
#define OUTZ_L_A 0x2C
#define OUTZ_H_A 0x2D


int16_t ax, ay, az; //Vars to hold accell data
byte axH, axL, ayH, ayL, azH, azL;

void setup() {
  Serial.begin(115200);
  Wire.begin();

}

void loop() {

  //X  Value
  Wire.beginTransmission(LSM);
  Wire.write(OUTX_L_A);
  Wire.endTransmission(false);
  Wire.requestFrom(LSM, 6, true);

  //X Value
  ax = (Wire.read()) | (Wire.read()<<8);
  Serial.print(ax); Serial.print(" ");

  //Y Value 
  ay = (Wire.read()) | (Wire.read()<<8);
  Serial.print(ay); Serial.print(" ");
  
  //Z Value 
  az = (Wire.read()) | (Wire.read()<<8);
  Serial.print(az); Serial.println("|"); 

  delay(500); 

}

void betterData(){

  //X  Value
  Wire.beginTransmission(LSM);
  Wire.write(OUTX_L_A);
  Wire.endTransmission(false);
  Wire.requestFrom(LSM, 6, true);

  axL = Wire.read();
  axH = Wire.read();
  ax = axL | (axH<<8);
  Serial.print(ax); Serial.print(" ");

  //Y Value 
  ayL = Wire.read();
  ayH = Wire.read();
  ay = ayL | (ayH<<8);
  Serial.print(ay); Serial.print(" ");
  
  //Z Value 
  azL = Wire.read(); 
  azH = Wire.read(); 
  az = azL | (azH<<8); 
  Serial.print(az); Serial.println("|"); 
  
}


void sadData(){

  //X  Value
  Wire.beginTransmission(LSM);
  Wire.write(OUTX_L_A);
  Wire.endTransmission(false);
  Wire.requestFrom(LSM, 2, true);

  axL = Wire.read();
  axH = Wire.read();
  ax = (axH << 8) | axL;
  Serial.print(ax); Serial.print(" ");

  //Y Value 
  Wire.beginTransmission(LSM);
  Wire.write(OUTY_L_A);
  Wire.endTransmission(false);
  Wire.requestFrom(LSM, 2, true);
  
  ayL = Wire.read();
  ayH = Wire.read();
  ay = (ayH << 8) | ayL;
  Serial.print(ay); Serial.print(" ");

  
  //Z Value 
  Wire.beginTransmission(LSM);
  Wire.write(OUTZ_L_A);
  Wire.endTransmission(false);
  Wire.requestFrom(LSM, 2, true);
  
  azL = Wire.read(); 
  azH = Wire.read(); 
  az = (azH << 8) | azL; 
  Serial.print(az); Serial.println("|"); 
  
}
