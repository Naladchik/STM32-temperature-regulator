#include "stm32f0xx.h"
#include "periphery.h"
#include "workflow.h"
#include "config.h"

extern float CurrentPower;
unsigned int HeaterOnTime = 0;
unsigned int HeaterOffTime = HEAT_CYCLE;
char HeaterStatus = 0;
char Mode = MODE_REGULATE;

void SetPower(float value){
  if(value < 0) CurrentPower = 0; else{
    if(value > 1.0) CurrentPower = 1.0; else{
      CurrentPower = value;
    }
  }
  float calc1, calc2;
  calc1 = CurrentPower;
  calc2 = (float)HEAT_CYCLE;
  calc1 = calc1 * calc2;
  HeaterOnTime = (unsigned int)calc1;
  HeaterOffTime = (unsigned int)(calc2 - calc1);
}

void LED_Control(char number, char status){
  switch(number){
    case(1):
      if(status == 0) LED_1_OFF else LED_1_ON;
      break;
    case(2):
      if(status == 0) LED_2_OFF else LED_2_ON;
      break;
    case(3):
      if(status == 0) LED_3_OFF else LED_3_ON;
      break;
    case(4):
      if(status == 0) LED_4_OFF else LED_4_ON;
      break;
    case(5):
      if(status == 0) LED_5_OFF else LED_5_ON;
      break;
    case(6):
      if(status == 0) LED_6_OFF else LED_6_ON;
      break;
    case(7):
      if(status == 0) LED_7_OFF else LED_7_ON;
      break;
    case(8):
      if(status == 0) LED_8_OFF else LED_8_ON;
      break;
  default:break;
  }
}

void LEDsControl(uint8_t byte){
  for(uint8_t i=1; i<=8; i++){
    if((byte & 0x80) > 0) LED_Control(i, ENABLE); else LED_Control(i, DISABLE);
    byte <<= 1;
  }
}

void ShowTempBar(uint8_t value){

}