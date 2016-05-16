#include <MCP23S17.h>

#include <SPI.h>
#define ADC_CS0 1 // top ADC
#define ADC_CS1 3 // bottom ADC
#define EXP_CS 4  // expander

MCP23S17 Bank1(&SPI, EXP_CS, 0);
MCP23S17 Bank2(&SPI, EXP_CS, 1);

IntervalTimer pollTimer; /// make this interrupt based. ie slave pulses T3, request state.?
const uint16_t poll_rate = 1500000;
volatile uint8_t flag;

void timerCallback() {
flag = true;
}

/*
 * (0,0,0)
 * GBP0 = output LED level
 * GBP1 = output LED mult
 * GBP2 = output LED A
 * GBP3 = output LED D
 * GBP4 = output LED S
 * GBP5 = output LED R
 * GBP6 = unused
 * GBP7 = unused
 * 
 * GPA0 = unused
 * GPA1 = input ENV button
 * GPA2 = input op4 button
 * GPA3 = input op3 button
 * GPA4 = input op2 button
 * GPA5 = input op1 button
 * GPA6 = input algo button
 * GPA7 = input wave button
 * 
 * (0,0,1)
 * GBP0 = output op1
 * GBP1 = output op2
 * GBP2 = output op3
 * GBP3 = output op4
 * GBP4 = unused
 * GBP5 = unused
 * GBP6 = unused
 * GBP7 = unused
 * 
 * GPA0 = unused
 * GPA1 = output algo LED 1.0 
 * GPA2 = output algo LED 1.1 
 * GPA3 = output algo LED 2.0
 * GPA4 = output algo LED 2.1 
 * GPA5 = output algo LED 3.0
 * GPA6 = output algo LED 3.1 
 * GPA7 = ouput env LED
 * 
 */

void setup() {

  delay(100);
  analogReference(EXTERNAL);
  analogReadRes(12);
  analogReadAveraging(16); 

  pinMode(ADC_CS0, OUTPUT);
  pinMode(ADC_CS1, OUTPUT);
  pinMode(EXP_CS, OUTPUT);

  digitalWrite(ADC_CS0, HIGH);
  digitalWrite(ADC_CS1, HIGH);
  
  Bank1.begin();
  delay(300);
  Bank2.begin();
  delay(300);
        
  for (int i = 0; i < 8; i++) {
        Bank1.pinMode(i, INPUT_PULLUP);   // expander 1: port A: inputs
        Bank1.pinMode(i+8, OUTPUT);       // port B: outputs
  }

  for (int i = 0; i < 16; i++) {
        Bank2.pinMode(i, OUTPUT);         // expander 2: only outputs
  }
 
 pollTimer.begin(timerCallback, poll_rate); // 
        

}

void loop() {
  
  
        if (flag) {
          
            flag = false;
            poll_matrix();
            
        }  
        
}

uint8_t _out;
uint8_t state_prev_;

void process_buttons(uint8_t inputs) {

 uint8_t _inputs = inputs;
 if (state_prev_ == _inputs)
  return;
  
 if (((_inputs >> 1) & 1u) == 0x0) 
  Serial.println("aux button");
 else if (((_inputs >> 2) & 1u) == 0x0) 
  Serial.println("op4");
 else if (((_inputs >> 3) & 1u) == 0x0) 
  Serial.println("op3");
 else if (((_inputs >> 4) & 1u) == 0x0) 
  Serial.println("op2");
 else if (((_inputs >> 5) & 1u) == 0x0) 
  Serial.println("op1");
 // 253 : env 0b11111101
 // 251 : op4 0b11111011
 // 247 : op3 0b11110111
 // 239 : op2 0b11101111
 // 223 : op1 0b11011111
  state_prev_ = _inputs;
}

void poll_matrix() {

   // port A inputs:
    uint16_t _inputs = Bank1.readPort(0);
    process_buttons(_inputs);
    //Serial.println(_inputs);

    // port B outputs:
    if (_out)
      _out = 0x00;
    else
      _out = 0xFF;
    
    Bank1.writePort(1, _out);
    Bank2.writePort(0, _out);
    Bank2.writePort(1, _out);
         
}



