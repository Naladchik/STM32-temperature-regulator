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

void UpdatePower(void){
  CurrentTemp = TEMP_RESOLITION * (float)RawTemp;
  eTemp = (float)TempSetpoint - CurrentTemp;
  if(StartCounter >= WAKE_UP){
    if(StartCounter >= START_INTEGRAL)Integral += eTemp;
    CurrentPower = K_PID * (eTemp + Integral/TI_PID);    
    SetPower(CurrentPower);        
  }
  if(StartCounter < HOUR) StartCounter++;
}
