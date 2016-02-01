#include <opl3.h>
#include <SPI.h>


#define ADC_CS0 1 // top ADC
#define ADC_CS1 3 // bottom ADC


SPISettings MCP3008(4000000, MSBFIRST, SPI_MODE0); 

opl3 opl3;

IntervalTimer seq_timer;
volatile uint8_t seq_time;
const uint32_t seq_rate = 100000;

void timerCallback()  
{ 
  seq_time = true;  
}

void setup() {

  delay(100);
  
  pinMode(ADC_CS0, OUTPUT);
  pinMode(ADC_CS1, OUTPUT);

  digitalWrite(ADC_CS0, HIGH);
  digitalWrite(ADC_CS1, HIGH);
  
  SPI.setMOSI(11);  
  SPI.setMISO(12);
  SPI.setSCK(13); 
  SPI.begin(); 

  opl3.OPL3_Init();
  delay(100);
  opl3.OPL3_SendDemoPatch();
  opl3.OPL3_OnFrame(); 

  /// 
  opl3.OPL3_SetOpl3Mode(1);
  
  opl3.OPL3_SetFourOp(0,1);
  opl3.OPL3_SetAlgorithm(0,4);
  
  opl3.OPL3_SetFourOp(2,1);
  opl3.OPL3_SetAlgorithm(2,2);
  
  opl3.OPL3_SetFMult(0, 6);
  opl3.OPL3_SetFMult(1, 12);
  opl3.OPL3_SetFMult(2, 3);
  opl3.OPL3_SetFMult(3, 4);
  
  opl3.OPL3_SetFMult(4, 6);
  opl3.OPL3_SetFMult(5, 2);
  opl3.OPL3_SetFMult(6, 1);
  opl3.OPL3_SetFMult(7, 3);
  
  opl3.OPL3_SetWaveform(0,0);
  opl3.OPL3_SetWaveform(1,0);
  opl3.OPL3_SetWaveform(2,0);
  opl3.OPL3_SetWaveform(3,0);
  
  opl3.OPL3_SetWaveform(4,0);
  opl3.OPL3_SetWaveform(5,0);
  opl3.OPL3_SetWaveform(6,0);
  opl3.OPL3_SetWaveform(7,0);
  
  opl3.OPL3_SetVolume(0,63);
  opl3.OPL3_SetVolume(1,63);
  opl3.OPL3_SetVolume(2,55);
  opl3.OPL3_SetVolume(3,60);
  
  opl3.OPL3_SetVolume(4,63);
  opl3.OPL3_SetVolume(5,50);
  opl3.OPL3_SetVolume(6,63);
  opl3.OPL3_SetVolume(7,63);

  opl3.OPL3_SetKSL(0,1);
  
  opl3.OPL3_SetAttack(0,2);
  opl3.OPL3_SetAttack(1,2);
  opl3.OPL3_SetAttack(2,2);
  opl3.OPL3_SetAttack(3,2);
  
  opl3.OPL3_SetAttack(4,2);
  opl3.OPL3_SetAttack(5,2);
  opl3.OPL3_SetAttack(6,2);
  opl3.OPL3_SetAttack(7,2);
  
  opl3.OPL3_SetDecay(0,1);
  opl3.OPL3_SetDecay(1,2);
  opl3.OPL3_SetDecay(2,1);
  opl3.OPL3_SetDecay(3,1);
  
  opl3.OPL3_SetDecay(4,1);
  opl3.OPL3_SetDecay(5,2);
  opl3.OPL3_SetDecay(6,3);
  opl3.OPL3_SetDecay(7,1);
  
  opl3.OPL3_SetSustain(0,1);
  opl3.OPL3_SetSustain(1,1);
  opl3.OPL3_SetSustain(2,11);
  opl3.OPL3_SetSustain(3,15);
  
  opl3.OPL3_SetSustain(4,1);
  opl3.OPL3_SetSustain(5,1);
  opl3.OPL3_SetSustain(6,11);
  opl3.OPL3_SetSustain(7,15);
  
  opl3.OPL3_DoSustain(0,0);
  opl3.OPL3_DoSustain(1,2);
  opl3.OPL3_DoSustain(2,4);
  opl3.OPL3_DoSustain(3,1);
  
  opl3.OPL3_DoSustain(4,0);
  opl3.OPL3_DoSustain(5,2);
  opl3.OPL3_DoSustain(6,4);
  opl3.OPL3_DoSustain(7,2);
  
  opl3.OPL3_SetRelease(0,12);
  opl3.OPL3_SetRelease(1,3);
  opl3.OPL3_SetRelease(2,1);
  opl3.OPL3_SetRelease(3,12);
  
  opl3.OPL3_SetRelease(4,12);
  opl3.OPL3_SetRelease(5,3);
  opl3.OPL3_SetRelease(6,1);
  opl3.OPL3_SetRelease(7,12);
  
  opl3.OPL3_SetFeedback(0, 2);
  
  opl3.OPL3_SetFeedback(2, 6);
  
  opl3.OPL3_OnFrame();
  seq_timer.begin(timerCallback, seq_rate);

  delay(200);
}

uint8_t seq[] =  {600, 500, 700, 1000, 600, 490, 660, 400, 490};
uint8_t seq2[] = {500, 600, 700, 800, 600, 490, 660, 400, 490};
uint8_t y, xxx; 
uint32_t timestamp, timestamp2;

void loop() {
  
  while(1) {

      if (seq_time) {
        timestamp2=micros();
        seq_time = false;
        y = y++ > 3 ? 0 : y;
        opl3.OPL3_SetFrequency(0, *(seq+y), 5);
        opl3.OPL3_SetFrequency(2, *(seq2+y), 4);
        
        uint8_t x = 8+random(3);
        xxx = 100+6*x;
        //Serial.print("param:...");Serial.println(x);
        opl3.OPL3_SetRelease(0, x);
        opl3.OPL3_SetRelease(1, x);
        opl3.OPL3_SetRelease(2, x);
        opl3.OPL3_SetRelease(3, x);

        opl3.OPL3_SetFMult(1, x-4);
        opl3.OPL3_SetFMult(3, x-4);
        opl3.OPL3_SetFMult(7, x-4);
        
        opl3.OPL3_SetRelease(4, x);
        opl3.OPL3_SetRelease(5, x);
        opl3.OPL3_SetRelease(6, x);
        opl3.OPL3_SetRelease(7, x);

        opl3.OPL3_SetSustain(3, 5+x);
        opl3.OPL3_SetSustain(7, 5+x);
       
        opl3.OPL3_Gate(0,0x1);
        opl3.OPL3_Gate(2,0x1);
        opl3.OPL3_OnFrame();
        //Serial.print("time wasted:...");Serial.println(micros()-timestamp2);
        timestamp = millis();
        read_MCP3008();
       }
  
     if (millis() - timestamp > xxx>>1) {
       opl3.OPL3_Gate(0,0x0);
       opl3.OPL3_Gate(2,0x0);
      
       opl3.OPL3_OnFrame();
      }  
  }
}
