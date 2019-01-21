#include "stm32f0xx.h"
#include "periphery.h"
#include "config.h"

extern char HeaterStatus;
extern unsigned int HeaterOnTime;
extern unsigned int HeaterOffTime;
char CommForTempConv = 0;
void TIM14_IRQHandler(void){
  if((HeaterOnTime == 0) || (HeaterOffTime == 0)){
    CommForTempConv = 1;
    TIM14->ARR = HEAT_CYCLE;
    if(HeaterOnTime == 0){
      RED_OFF;
      BLUE_ON;
      HEAT_OFF;
    }
    if(HeaterOffTime == 0){
      RED_ON;
      BLUE_OFF;
      HEAT_ON;
    }
  }else{
    if(HeaterStatus == 0){
      RED_ON;
      BLUE_OFF;
      HEAT_ON;
      TIM14->ARR = HeaterOnTime;
      HeaterStatus = 1;
      CommForTempConv = 1;
    }else{
      RED_OFF;
      BLUE_ON;
      HEAT_OFF;
      TIM14->ARR = HeaterOffTime;
      HeaterStatus = 0;
    }
  }
  
  TIM_ClearFlag(TIM14, TIM_FLAG_Update);
}


extern char Blink;
char ButtonStatus = 0;
void TIM16_IRQHandler(void){
  static int SecCounter = 0;
  static char ButtonCounter = 0;
  if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3) == 0){
    //button pressed
    if(ButtonCounter == JITTER){
      ButtonStatus = 1;
    }else{
      ButtonCounter++;
    }
  }else{
    //button released
    ButtonCounter = 0;
    ButtonStatus = 0;
  }
  if(SecCounter == SEC_CONST){
    SecCounter = 0;
    if(Blink == 0) Blink = 1; else Blink = 0;
  }else SecCounter++;
  TIM_ClearFlag(TIM16, TIM_FLAG_Update);
}


char BuzzerOnOff = 0;
void TIM17_IRQHandler(void){
  static char BuzzerStatus = 0; 
  if(BuzzerStatus == 0){
    if(BuzzerOnOff != 0) BUZZ_ON;
    BuzzerStatus = 1;
  }else{
    BUZZ_OFF;
    BuzzerStatus = 0;    
  }  
  TIM_ClearFlag(TIM17, TIM_FLAG_Update);
}
