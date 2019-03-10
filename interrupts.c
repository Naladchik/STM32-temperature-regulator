#include "stm32f0xx.h"
#include "periphery.h"
#include "config.h"
#include "types.h"

extern FlagsTypeDef    Flags;
extern PowerTypeDef    Power;
extern ButtonTypeDef   Button;
extern TimersTypeDef   Timers;
void TIM14_IRQHandler(void){ //Heater control (1 sec cycle)
  if(!Flags.ErrorGlobal){
    if((Power.HeaterOnTime == 0) || (Power.HeaterOffTime == 0)){
      Flags.AcquireTemperature = ENABLE;
      TIM14->ARR = HEAT_CYCLE;
      if(Power.HeaterOnTime == 0){HEAT_OFF; Flags.Heater = DISABLE;}
      if(Power.HeaterOffTime == 0){HEAT_ON; Flags.Heater = ENABLE;}
    }else{
      if(Power.HeaterStatus){
        HEAT_OFF; Flags.Heater = DISABLE;
        TIM14->ARR = Power.HeaterOffTime;
        Power.HeaterStatus = DISABLE;
      }else{
        HEAT_ON; Flags.Heater = ENABLE;
        TIM14->ARR = Power.HeaterOnTime;
        Power.HeaterStatus = ENABLE;
        Flags.AcquireTemperature = ENABLE;        
      }
    }
  }else{
    HEAT_OFF; Flags.Heater = DISABLE;
    if(Timers.Show)Timers.Show = DISABLE; else Timers.Show = ENABLE;
  }
  TIM_ClearFlag(TIM14, TIM_FLAG_Update);
}

extern TimersTypeDef Timers;
extern ButtonTypeDef Button;
void TIM16_IRQHandler(void){ //Button control (1000 us)
  if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3) == 0){
    //button pressed
    if(Button.JitterTimer >= JITTER){
      Button.PressedNow = ENABLE;
      if(Button.PressTimer < MINUTE)Button.PressTimer++;
      Button.ReleaseTimer = 0;
    }else{
      Button.JitterTimer++;
    }
  }else{
    //button released
    Button.JitterTimer = 0;
    if(Button.ReleaseTimer < MINUTE)Button.ReleaseTimer++;
    Button.PressTimer = 0;
    Button.PressedNow = DISABLE;
  }
  Timers.FromStart++;
  if(Timers.FromStart == MINUTE) Timers.FromStart = 0;
  TIM_ClearFlag(TIM16, TIM_FLAG_Update);
}


void TIM17_IRQHandler(void){ // Buzzer control
  static uint8_t BuzzerState = 0; 
  if(BuzzerState == 0){
    if(Flags.Buzzer) BUZZ_ON;
    BuzzerState = 1;
  }else{
    BUZZ_OFF;
    BuzzerState = 0;    
  }
  if(Flags.BeepCounter > 0) Flags.BeepCounter--;
  if(Flags.BeepCounter == 1) Flags.Buzzer = DISABLE;
  TIM_ClearFlag(TIM17, TIM_FLAG_Update);
}
