void setup() {
Serial.begin(115200);

byte a = 0x21;
byte b = 0x10;

int16_t c = ((b & 0xFF) << 8) | (a & 0xFF);
int16_t d = ((a & 0xFF) << 8) | (b & 0xFF);

Serial.println(c);
Serial.println(d);

}

void loop() {
  // put your main code here, to run repeatedly:

}
