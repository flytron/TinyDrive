
// ************************************************************
// ***************   TinyDrive Default Code   *****************
// *****  TinyDrive Designed by Melih Karakelle on 2020   *****
// ***       an Arudino based Dual Motor Driver board       ***
// ***         This Source code licensed under GPL          ***
// ************************************************************
// Firmware Version Number: 1.12
// Latest Code Update     : 2021-01-25
// Supported Hardwares    : TinyDrive v1 (store.flytron.com)
// Project Forum          : https://www.facebook.com/groups/181517579149618
// GitHub Page            : https://github.com/flytron/TinyDrive
// ************************************************************

// # PROJECT DEVELOPERS # 
// Melih Karakelle (http://store.flytron.com)


//**** Define Hardware Pins ****
//******************************

#define STATUS_LED 13      // On Board LED pin

#define M1_PHASE 5        // Motor 1 Direction Control Pin
#define M1_ENABLE 6     // Motor 1 PWM Control Pin

#define M2_PHASE 8        // Motor 2 Direction Control Pin
#define M2_ENABLE 9     // Motor 2 PWM Control Pin

#define SERVO_1 A4      // CH4 Servo PWM signal output.
#define SERVO_2 A5      // CH5 Servo PWM signal output.

#define SBUS_ENABLE_PIN A3  // HIGH: Enables the XOR gate to invert the SBUS signal to Standard Serial.  LOW: Standard Serial Input mode. 

#define INT_TX 2            // D2(INT0) pin connected to D1(Tx) on board to simplify pinouts. Serial Tx pin must be disabled before activate the INT0 

#define HEADLIGHT_PIN 3     // D3(PWM) pin connected to the Base pin of an NPN transistor. [LED+] has a serial 10 Ohm resistor to limit the current. Use AnalogWrite(0-255) to control the light intensity. 

#define Battery_Sensor_Pin  A6        // Connected to the Battery input over 1/11 voltage divider. 
#define Battery_Sensor_Factor  11.6   // Multiply the reading with this value and you will find the battery voltage in mV  
#define Cell_Min_Voltage 3200         // Minimum 3.2V per cell when the motors off.

// Simplified Status LED Modes 
#define LED_TOGGLE digitalWrite(STATUS_LED,!digitalRead(STATUS_LED));
#define LED_ON digitalWrite(STATUS_LED,HIGH);
#define LED_OFF digitalWrite(STATUS_LED,LOW);

/***********************************/
uint8_t Mixer_Mode = 0;               // 0: Mixed (CH1+CH2) motor outputs. 1: Direct Motor outputs(CH1=Motor1, CH2=Motor2)

volatile int _channels[18];           // RC Receiver Channel values(0-2047). the center=1023 
volatile boolean new_sbus_data = 0;   // New SBUS data received
volatile boolean new_ppm_data = 0;    // New PPM data received

volatile boolean motors_off = 1;

uint16_t Battery_Voltage_mV = 0;      // The battery voltage in mV
uint8_t Battery_Cell_Count = 1;       // Detected battery cell count (1 or 2)


#define CHECK_PERIOD 1000L              // Check the battery level in every 1000ms
unsigned long last_check_time = 0L ;  



/************************************************************************/
/* the setup function runs once when you press reset or power the board */
/************************************************************************/

void setup() {
  
  analogReference(INTERNAL); // Enable internal 1.1v analog reference for the battery voltage readings
  
  
  pinMode(STATUS_LED, OUTPUT);  // initialize the LED pin as an output.
  LED_ON;  


  pinMode(M1_PHASE, OUTPUT);
  pinMode(M1_ENABLE, OUTPUT);

  pinMode(M2_PHASE, OUTPUT);
  pinMode(M2_ENABLE, OUTPUT);  

  pinMode(HEADLIGHT_PIN, OUTPUT);
  digitalWrite(HEADLIGHT_PIN,LOW);

  digitalWrite(M1_PHASE,LOW);
  digitalWrite(M2_PHASE,LOW);

  digitalWrite(M1_ENABLE,LOW);
  digitalWrite(M2_ENABLE,LOW);

  mixer_mode_check(); // check the short circuit between SDA & SCL pads  to toggle bewteen the motor mixer modes. 

  pinMode(SERVO_1, OUTPUT);
  pinMode(SERVO_2, OUTPUT);
  digitalWrite(SERVO_1,LOW);
  digitalWrite(SERVO_2,LOW);

  Serial.begin(115200, SERIAL_8E1);
  
  delay(200); // Wait 200mS on the startup for the voltage stabilisation. 
   
  Battery_Voltage_mV =  analogRead(Battery_Sensor_Pin) * Battery_Sensor_Factor; // Read battery voltage in mV
  delay(100);
  // Read the battery voltage twice to avoid the ADC buffer problems. 
  // The first read may be faulty due to the ADC referance change on the startup. The next read will be fine.  
  Battery_Voltage_mV =  analogRead(Battery_Sensor_Pin) * Battery_Sensor_Factor; // Read battery voltage in mV
  //Serial.println(Battery_Voltage_mV ); // Print the battery reading to debug.
  

  if (Battery_Voltage_mV>6200) 
      {
        Battery_Cell_Count = 2;  // Detect 2S liPo if >6.2v
      }
      else
      { 
        Battery_Cell_Count = 1;  // Run on 1S LiPo mode for 1S LiPo or 3-4x AA Batteries(<=6V) 
      }

  delay(100);                                     
  SBUS_begin(); // initialize the SBUS input
  PPM_Init();   // initialize the PPM input
  
  march();   // Play The Imperial March :)  You can comment out this line or enable the extended version from the Mach function. 

  last_check_time = millis (); // set the first battery check time.

  LED_OFF; 

}

// the loop function runs over and over again forever
void loop() {
  
  SBUS_process(); // Check for the SBUS data 
  PPM_process();  // Check for the PPM data

  if (new_sbus_data || new_ppm_data)
    {
      new_sbus_data = 0; 
      new_ppm_data = 0;
      
      int ch3 = _channels[2]; //Headlights Channel
      int ch4 = _channels[3]; // Servo 1 output
      int ch5 = _channels[4]; // Servo 2 output
      
      if (ch4>2048) ch4 = 2048; 
      if (ch5>2048) ch5 = 2048; 
      if (ch4<0) ch4 = 0; 
      if (ch5<0) ch5 = 0;             
       
      Motor_Mixed(Mixer_Mode);  // Motor outputs   

      //Headlight 
      if (ch3>580)
        {
          if (ch3>1600) ch3 = 1600; // Full Power at >1600
          analogWrite(HEADLIGHT_PIN, (ch3-580)/4);  // 0-255  
        }  
        else
        analogWrite(HEADLIGHT_PIN, 0);  

      //Servo outs (1000-2000uS PWM)
      digitalWrite(SERVO_1,HIGH);
      delayMicroseconds((ch4/2) + 1000);
      digitalWrite(SERVO_1,LOW);

      digitalWrite(SERVO_2,HIGH);
      delayMicroseconds((ch5/2) + 1000);
      digitalWrite(SERVO_2,LOW);

      LED_OFF;  
    }
  
   delay(10);

   // Check for the battery voltage 
   if ((millis() - last_check_time) >= CHECK_PERIOD)
       {
        last_check_time = millis ();
        
        if (motors_off) // Measure the battery when the motors off. 
            {
            Battery_Voltage_mV = analogRead(Battery_Sensor_Pin) * Battery_Sensor_Factor; // in mV
    
            if (Battery_Voltage_mV< (Battery_Cell_Count* Cell_Min_Voltage)) // stop working below the minimum cell voltage and blink the LED. 
               {
                while(1) // Infinite loop 2 short blink 1 second OFF
                {
                  LED_ON
                  delay(100);
                  LED_OFF
                  delay(100);
                  LED_ON
                  delay(100);
                  LED_OFF
                  delay(1000);
                }
                
               }
            }   
       }
       
      
}
