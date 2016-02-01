
const uint16_t numADC = 16;

uint8_t ADC_counter; 
uint16_t adc_val[numADC]; // store 


void read_MCP3008() {
   
      ADC_counter++;
    
      if (ADC_counter < numADC) {
           uint8_t n = ADC_counter;
           adc_val[n] = readADC(n); 
      }
      else {
           uint8_t n = ADC_counter = 0;
           adc_val[n] = readADC(n);
           
           for (int i = 0; i < numADC; i++) {
                 Serial.print(adc_val[i]);
                 Serial.print(" | ");
           }
           Serial.println(" ");  
      }    
}


/* read ADC */

uint16_t readADC(int16_t _channel){ 

 
        unsigned char spi_data[3];
        int16_t _tmp = 0x0, _ch = _channel;

        if (_ch < 8) _ch += 0x8;  // see spi_data[1]
        else _tmp = 0x1; 
    
        spi_data[0] = 0x01; // start flag
        spi_data[1] =  _ch << 4; // should be: (_ch + 0x8) << 4 for a single mcp3008; (but de facto we have 16 channels)
        spi_data[2] = 0x00;

        SPI.beginTransaction(MCP3008); 

        // switch ADCs:
        _tmp ? digitalWriteFast(ADC_CS0, LOW) : digitalWriteFast(ADC_CS1, LOW);
      
        SPI.transfer(spi_data[0]);
        byte _msb = SPI.transfer(spi_data[1]);
        byte _lsb = SPI.transfer(spi_data[2]);
   
        _tmp ? digitalWriteFast(ADC_CS0, HIGH) : digitalWriteFast(ADC_CS1, HIGH);
        
        SPI.endTransaction(); 
        _tmp = 1023 - ((_msb & 3) << 8 | _lsb); 
        return _tmp;
}


