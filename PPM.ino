unsigned long int a,b,c;
int x[15],ch1[15],ch[7],i;
volatile boolean new_ppm_received = 0;
//specifing arrays and variables to store values 

void PPM_Init() {
  UCSR0B &= ~(1<<TXEN0);  //disable TX
  pinMode(1, INPUT); // disable TX pin  
  
  pinMode(INT_TX, INPUT_PULLUP); // enable interrupt pin pull up
  attachInterrupt(digitalPinToInterrupt(2), Read_PPM, RISING);
  // enabling interrupt at pin 2
}

void Read_PPM()  {  
    //measure the distance between two rising PPM signal in microseconds
    
    a=micros(); //store time value a
    c=a-b;      //calculating time inbetween two peaks
    b=a;        //store the current a time in b 
    x[i]=c;     //store the time difference in the array
    i=i+1;        
    if(i==15){ 
                for(int j=0;j<15;j++) {ch1[j]=x[j];} //copy & store all values from the temporary array to [ch] array. 
                i=0; 
                new_ppm_received = 1;
             }             
} 

             
void PPM_process(){

      if (!new_ppm_received) return; // no data, return immediately.
      new_ppm_received = 0;      
       
      int i,j,k=0;
      for(k=14;k>-1;k--){if(ch1[k]>10000){j=k;}}  //detecting separation space 10000us in that another array                     
      for(i=0;i<=5;i++) //assign 6 channel values after separation space
          {
          if (ch1[i+j+1]<1000) ch1[i+j+1] = 1000; // be sure PPM channel value above 1000uS
          if (ch1[i+j+1]>2000) ch1[i+j+1] = 2000; // be sure PPM channel value below 2000uS             
          _channels[i]=2*(ch1[i+j+1]-1000);  // convert the PPM uS values to SBUS type values (0-2000)                    
          }
      
        new_ppm_data = 1; 
        LED_ON;
}     
