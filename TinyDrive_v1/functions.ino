#include <EEPROM.h>

#define motor_mixer_addr  0 /** motor Mixer Settings EEPROM address **/


void mixer_mode_check()
{
  Mixer_Mode = EEPROM.read(motor_mixer_addr);
  if (Mixer_Mode>5) Mixer_Mode = 0; // set to default if eeprom is factory default (0xFF)


  pinMode(A4, OUTPUT); 
  digitalWrite(A4,LOW);
  pinMode(A5, INPUT_PULLUP);

  if (!digitalRead(A5)) // shortcut detected between pad 1 and 2 switch the mixer mode. 
     {
      Mixer_Mode = (Mixer_Mode+1) % 2; // toggle between 0-1
      EEPROM.write(motor_mixer_addr, Mixer_Mode);

      while(1)
            {
              LED_ON
              delay(100);
              LED_OFF
              delay(1000);
            }
     }

}


// STARWARS IMPERIAL MARCH :)))
void march_beep ( int frequencyInHertz, long timeInMilliseconds)
{ 
    digitalWrite(M1_ENABLE,HIGH);
    digitalWrite(M2_ENABLE,HIGH); 
    int x;   
    long delayAmount = (long)(1000000/frequencyInHertz);
    long loopTime = (long)((timeInMilliseconds*1000)/(delayAmount*2));
    for (x=0;x<loopTime;x++)   
    {    
        digitalWrite(M1_PHASE,HIGH);
        digitalWrite(M2_PHASE,HIGH);
        delayMicroseconds(delayAmount);
        digitalWrite(M1_PHASE,LOW);
        digitalWrite(M2_PHASE,LOW);
        delayMicroseconds(delayAmount);
    }    

    digitalWrite(M1_ENABLE,LOW);
    digitalWrite(M2_ENABLE,LOW);    
    delay(20);
    //a little delay to make all notes sound separate
}    
     
void march()
{    
    #define c 261
    #define d 294
    #define e 329
    #define f 349
    #define g 391
    #define gS 415
    #define a 440
    #define aS 455
    #define b 466
    #define cH 523
    #define cSH 554
    #define dH 587
    #define dSH 622
    #define eH 659
    #define fH 698
    #define fSH 740
    #define gH 784
    #define gSH 830
    #define aH 880
        
    march_beep(a, 500); 
    march_beep(a, 500);     
    march_beep(a, 500); 
    march_beep(f, 350); 
    march_beep(cH, 150);
    
    march_beep(a, 500);
    march_beep(f, 350);
    march_beep(cH, 150);
    march_beep(a, 1000);
    //first bit
    
    march_beep(eH, 500);
    march_beep(eH, 500);
    march_beep(eH, 500);    
    march_beep(fH, 350); 
    march_beep(cH, 150);
    
    march_beep(gS, 500);
    march_beep(f, 350);
    march_beep(cH, 150);
    march_beep(a, 1000);
    //second bit...
 /*   
    march_beep(aH, 500);
    march_beep(a, 350); 
    march_beep(a, 150);
    march_beep(aH, 500);
    march_beep(gSH, 250); 
    march_beep(gH, 250);
    
    march_beep(fSH, 125);
    march_beep(fH, 125);    
    march_beep(fSH, 250);
    delay(250);
    march_beep(aS, 250);    
    march_beep(dSH, 500);  
    march_beep(dH, 250);  
    march_beep(cSH, 250);  
    //start of the interesting bit
    
    march_beep(cH, 125);  
    march_beep(b, 125);  
    march_beep(cH, 250);      
    delay(250);
    march_beep(f, 125);  
    march_beep(gS, 500);  
    march_beep(f, 375);  
    march_beep(a, 125); 
    
    march_beep(cH, 500); 
    march_beep(a, 375);  
    march_beep(cH, 125); 
    march_beep(eH, 1000); 
    //more interesting stuff (this doesn't quite get it right somehow)
    
    march_beep(aH, 500);
    march_beep(a, 350); 
    march_beep(a, 150);
    march_beep(aH, 500);
    march_beep(gSH, 250); 
    march_beep(gH, 250);
    
    march_beep(fSH, 125);
    march_beep(fH, 125);    
    march_beep(fSH, 250);
    delay(250);
    march_beep(aS, 250);    
    march_beep(dSH, 500);  
    march_beep(dH, 250);  
    march_beep(cSH, 250);  
    //repeat... repeat
    
    march_beep(cH, 125);  
    march_beep(b, 125);  
    march_beep(cH, 250);      
    delay(250);
    march_beep(f, 250);  
    march_beep(gS, 500);  
    march_beep(f, 375);  
    march_beep(cH, 125); 
           
    march_beep(a, 500);            
    march_beep(f, 375);            
    march_beep(c, 125);            
    march_beep(a, 1000);      
    */ 
    //and we're done \ó/    
}
