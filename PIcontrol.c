#include "stm32f0xx.h"
#include "periphery.h"
#include "workflow.h"
#include "config.h"

char CommForPIcontrol = 0;
float CurrentPower = 0.0; //Current power from 0.0 to 1.0
//Source temperatures
char TempSetpoint = DEFAULT_TEMP;
int RawTemp;
float CurrentTemp, eTemp;
float Integral = 0;
unsigned int StartCounter = 0;
char PreHeat = 1;

void Calc_eTemp(void){
  CurrentTemp = TEMP_RESOLITION * (float)RawTemp;
  eTemp = (float)TempSetpoint - CurrentTemp;
}

void UpdatePower(void){
  if(StartCounter >= WAKE_UP){
    Integral += eTemp;
    CurrentPower = K_PID * (eTemp + Integral/TI_PID);    
    SetPower(CurrentPower);
  }
  if(StartCounter < HOUR) StartCounter++;
}

void HeatUp(void){
  static int PreHeatDope;
  if(StartCounter == WAKE_UP){
    if(eTemp > COLD_DELTA){
      PreHeatDope = (int)(PREHEAT_K * eTemp);
      SetPower(1.00);
    }else PreHeat = 0;    
  }
  if(StartCounter == (WAKE_UP + (int)PreHeatDope)){
    SetPower(0.00);    
  }
  if(StartCounter == (WAKE_UP + (int)PreHeatDope) + PREHEAT_TIME) PreHeat = 0;
  if(StartCounter < HOUR) StartCounter++;
}
