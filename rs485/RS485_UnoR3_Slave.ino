#include <SimpleModbusSlave.h>
#include <Wire.h>
#include "rgb_lcd.h"

rgb_lcd lcd;

const int colorR = 255;
const int colorG = 0;
const int colorB = 0;

#define  LED 8  

//////////////// registers of your slave ///////////////////
enum 
{
  reg0_VAL,     
  reg1_VAL,        
  HOLDING_REGS_SIZE
};

unsigned int holdingRegs[HOLDING_REGS_SIZE]; // function 3 and 16 register array
////////////////////////////////////////////////////////////

void setup()
{
    lcd.begin(16, 2);
    lcd.setRGB(colorR, colorG, colorB);
    lcd.print("Receiving from Master...");
  //This line apply to get function & register from master
  //modbus_configure(&Serial, baud, SERIAL_8N2: 8bits, non-parity-2 bit stop, scan rate, pin2, HOLDING_REGS_SIZE, holdingRegs);
  modbus_configure(&Serial, 9600, SERIAL_8N2, 1, 2, HOLDING_REGS_SIZE, holdingRegs);
  
  //this line apply to send response to Master modbus_update_comms(9600, SERIAL_8N2, ID_slave: 1);
  modbus_update_comms(9600, SERIAL_8N2, 1);
  
  pinMode(LED, OUTPUT);
}

void loop()
{

  modbus_update();
//  holdingRegs[ADC_VAL] = analogRead(A0);
  
 digitalWrite(LED, HIGH);
 delay(holdingRegs[reg0_VAL]);
 digitalWrite(LED, LOW);
 delay(holdingRegs[reg0_VAL]);
 
 
 lcd.setCursor(0, 1);
 lcd.print(holdingRegs[reg0_VAL]);
 lcd.print("  "); 
 lcd.setCursor(5, 1);
 lcd.print(holdingRegs[reg0_VAL]>>2); 
 lcd.print("  ");  
 lcd.setCursor(10, 1);
 lcd.print(holdingRegs[reg0_VAL]>>4);
 lcd.print("  "); 


}
