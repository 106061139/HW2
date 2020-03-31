#include "mbed.h"

Serial pc( USBTX, USBRX );

AnalogOut Aout(DAC0_OUT);

AnalogIn Ain(A0);


int sample = 512;

int i;
float f;
DigitalOut redLED(LED1);
DigitalOut greenLED(LED2);
float ADCdata[512];
BusOut display(D6, D7, D9, D10, D11, D5, D4, D8);
DigitalIn  Switch(SW3);
char table[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
char ptable[10] = {0xBF, 0x86, 0xDB, 0xAF, 0xE6, 0xED, 0xFD, 0x87, 0xFF, 0xEF};

int main(){
  int d1=0,d2=0;
  int fi;
  int a[3];
  while(1){
  for (i = 0; i < sample; i++){

    ADCdata[i] = Ain;

    wait(1./sample);
  }

  for (i = 1; i < sample; i++){
    if(ADCdata[i]>ADCdata[i+1]&&ADCdata[i]>ADCdata[i-1]){
      if(d1==0) d1=i;
      else if(d2==0) {
        d2=i;
        f=1./(1./sample*(d2-d1));
        break;
    }
  }
  }
  for (i = 0; i < sample; i++){

    pc.printf("%1.3f\r\n", ADCdata[i]);

    wait(0.001);

  }
  fi=(int)(f+0.5);
  a[0]=fi/100;
  a[1]=(fi/10)%10;
  a[2]=fi%10;
 // pc.printf("frequency %d",fi);
  while(1){  
  //  Aout=Ain;
    if(!Switch){
      redLED=0;
      greenLED=1;
        for(i=0;i<3;i++){
          if(i==2){
          display=ptable[a[i]];
          }
          else{
           display=table[a[i]]; 
          }
          wait(0.4);
        }
    }
    else
    {
      redLED=1;
      greenLED=0;
      display=0x00;
    }
    float i;
    for( i=0; i<2; i+=0.05){

      Aout = 0.5 + 0.5*sin(i*3.14159);

      wait(0.05/(2*f));

    }
  }
  }
}
