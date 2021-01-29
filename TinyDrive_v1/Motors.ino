

void Motor_Mixed(uint8_t Mixer_M) {

      int ch1 = _channels[0];
      int ch2 = _channels[1];

      int m_throttle = 1023-ch2;
      int m_direction = 1023-ch1;

      int motor1_mix =0;   
      int motor2_mix =0;

      if ((abs(m_throttle) < 30) && (abs(m_direction) < 30)) //central dead zone
        {
          motors_off = 1;
          m_throttle = 0;
          m_direction = 0;
        }
        else
        motors_off = 0;

       
      // --- Motor Mixer for Dual Motors----
      if (Mixer_M==0)
          {
            motor1_mix = (m_throttle+m_direction)/3;   
            motor2_mix = (m_throttle-m_direction)/3;    
          }
      // --- Non mixed motor outputs ----
      if (Mixer_M==1)
          {
            motor1_mix = m_throttle /2;   
            motor2_mix = m_direction /2;    
          }
      
    
      if (motor1_mix>255)  motor1_mix = 255;
      if (motor1_mix<-255) motor1_mix = -255;
      if (motor2_mix>255)  motor2_mix = 255;
      if (motor2_mix<-255) motor2_mix = -255;
      
      if (motor1_mix>0) digitalWrite(M1_PHASE,LOW); else digitalWrite(M1_PHASE,HIGH); //Motor 1 Phase/Direction
      if (motor2_mix>0) digitalWrite(M2_PHASE,HIGH); else digitalWrite(M2_PHASE,LOW); //Motor 2 Phase/Direction

      analogWrite(M1_ENABLE, abs(motor1_mix)); // Motor 1 PWM
      analogWrite(M2_ENABLE, abs(motor2_mix)); // Motor 2 PWM
}
