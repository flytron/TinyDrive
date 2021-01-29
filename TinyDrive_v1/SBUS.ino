
#define SBUS_FAILSAFE_INACTIVE 0
#define SBUS_FAILSAFE_ACTIVE   1
#include <avr/io.h>

#define SBUS_STARTBYTE         0x0f
#define SBUS_ENDBYTE           0x00


void SBUS_begin() {
  for (byte i = 0; i<18; i++) {
    _channels[i]      = 1023; //default values
  }
  Serial.begin(100000, SERIAL_8E2);
    
  pinMode(SBUS_ENABLE_PIN, OUTPUT);
  digitalWrite(SBUS_ENABLE_PIN,HIGH); 
}

void SBUS_process() {
  static byte buffer[25];
  static byte buffer_index = 0;

  while (Serial.available()) {
    byte rx = Serial.read();
    if (buffer_index == 0 && rx != SBUS_STARTBYTE) {
      //incorrect start byte, out of sync
      continue;
    }

    buffer[buffer_index++] = rx;

    if (buffer_index == 25) {
      buffer_index = 0;
      if (buffer[24] != SBUS_ENDBYTE) {
        //incorrect end byte, out of sync
        continue;
      }

      _channels[0]  = ((buffer[1]    |buffer[2]<<8)                 & 0x07FF);
      _channels[1]  = ((buffer[2]>>3 |buffer[3]<<5)                 & 0x07FF);
      _channels[2]  = ((buffer[3]>>6 |buffer[4]<<2 |buffer[5]<<10)  & 0x07FF);
      _channels[3]  = ((buffer[5]>>1 |buffer[6]<<7)                 & 0x07FF);
      _channels[4]  = ((buffer[6]>>4 |buffer[7]<<4)                 & 0x07FF);
      _channels[5]  = ((buffer[7]>>7 |buffer[8]<<1 |buffer[9]<<9)   & 0x07FF);
      _channels[6]  = ((buffer[9]>>2 |buffer[10]<<6)                & 0x07FF);
      _channels[7]  = ((buffer[10]>>5|buffer[11]<<3)                & 0x07FF);
      _channels[8]  = ((buffer[12]   |buffer[13]<<8)                & 0x07FF);
      _channels[9]  = ((buffer[13]>>3|buffer[14]<<5)                & 0x07FF);
      _channels[10] = ((buffer[14]>>6|buffer[15]<<2|buffer[16]<<10) & 0x07FF);
      _channels[11] = ((buffer[16]>>1|buffer[17]<<7)                & 0x07FF);
      _channels[12] = ((buffer[17]>>4|buffer[18]<<4)                & 0x07FF);
      _channels[13] = ((buffer[18]>>7|buffer[19]<<1|buffer[20]<<9)  & 0x07FF);
      _channels[14] = ((buffer[20]>>2|buffer[21]<<6)                & 0x07FF);
      _channels[15] = ((buffer[21]>>5|buffer[22]<<3)                & 0x07FF);

      ((buffer[23])      & 0x0001) ? _channels[16] = 2047: _channels[16] = 0;
      ((buffer[23] >> 1) & 0x0001) ? _channels[17] = 2047: _channels[17] = 0;

      new_sbus_data = 1;
      LED_ON;
      //Serial.println(_channels[2]); // Print Throttle
    }
  }
}
