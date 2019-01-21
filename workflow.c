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


void SelfTest(void){
      HEAT_ON;
      Delay_ms(SELF_TEST_PACE);
      HEAT_OFF;
      RED_ON;
      Delay_ms(SELF_TEST_PACE);
      RED_OFF;
      GREEN_ON;
      Delay_ms(SELF_TEST_PACE);
      GREEN_OFF;
      BLUE_ON;
      Delay_ms(SELF_TEST_PACE);
      BLUE_OFF;
      LED_1_ON;
      Delay_ms(SELF_TEST_PACE);
      LED_1_OFF;
      LED_2_ON;
      Delay_ms(SELF_TEST_PACE);
      LED_2_OFF;
      LED_3_ON;
      Delay_ms(SELF_TEST_PACE);
      LED_3_OFF;
      LED_4_ON;
      Delay_ms(SELF_TEST_PACE);
      LED_4_OFF;
      LED_5_ON;
      Delay_ms(SELF_TEST_PACE);
      LED_5_OFF;
      LED_6_ON;
      Delay_ms(SELF_TEST_PACE);
      LED_6_OFF;
      LED_7_ON;
      Delay_ms(SELF_TEST_PACE);
      LED_7_OFF;
      LED_8_ON;
      Delay_ms(SELF_TEST_PACE);
      LED_8_OFF;
      BuzzerControl(ENABLE);
      Delay_ms(SELF_TEST_PACE);
      BuzzerControl(DISABLE);
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

void LEDsBar(char count){
  char i;
  if(count <= 0){
    for(i = 1; i <= 8; i++) LED_Control(i, DISABLE);
  }else{
    if(count >= 8){
      for(i = 1; i <= 8; i++) LED_Control(i, ENABLE);
    }else{
      for(i = 1; i <= 8; i++){
        if(i <= count) LED_Control(i, ENABLE); else LED_Control(i, DISABLE);
      }      
    }
  }  
}


void ShowLED_Regulate(float _TempSetpoint, unsigned int _RawTemp){
  extern char Blink;
  float _ActualTemp;
  _ActualTemp = (float)_RawTemp;
  _ActualTemp *= TEMP_RESOLITION;
  if(_ActualTemp >= _TempSetpoint){
    if(_ActualTemp - _TempSetpoint < 0.5) LEDsBar((int)_TempSetpoint - MIN_TEMP + 1);else{
      LEDsBar((int)_TempSetpoint - MIN_TEMP);
      if(Blink == 0){
        LED_Control((int)_TempSetpoint - MIN_TEMP + 1, DISABLE); 
      }else{
        LED_Control((int)_TempSetpoint - MIN_TEMP + 1, ENABLE);
      }
      for(char i = (int)_TempSetpoint - MIN_TEMP + 2; i <= 8; i++){
        if(i  > (int)_ActualTemp - MIN_TEMP + 2) LED_Control(i, DISABLE); else LED_Control(i, ENABLE);
      }
    }
  }else{
    if(_TempSetpoint - _ActualTemp < 0.5) LEDsBar((int)_TempSetpoint - MIN_TEMP + 1);else{
      if((int)_ActualTemp >= MIN_TEMP) LEDsBar((int)_ActualTemp - MIN_TEMP + 1); else LEDsBar(0);
      if(Blink == 0){
        LED_Control((int)_TempSetpoint - MIN_TEMP + 1, DISABLE); 
      }else{
        LED_Control((int)_TempSetpoint - MIN_TEMP + 1, ENABLE);
      }
    }
  }
}