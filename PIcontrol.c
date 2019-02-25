#include "stm32f0xx.h"
#include "periphery.h"
#include "workflow.h"
#include "config.h"

extern uint8_t RGB_status;
char CommForPIcontrol = 0;
float CurrentPower = 0.0; //Current power from 0.0 to 1.0
//Source temperatures
char TempSetpoint = DEFAULT_TEMP;
int RawTemp;
float eTemp;
float Integral = 0;
unsigned int StartCounter = 0;
char PreHeat = 1;

void UpdatePower(void){
  if(StartCounter >= WAKE_UP){
    if(RGB_status == 0)RGB_status = RGB_RED;
    if((CurrentPower < 1.0) && (CurrentPower > 0.0)) Integral += eTemp;    
    CurrentPower = K_PID * (eTemp + Integral/TI_PID);    
    SetPower(CurrentPower);
  }
  if(StartCounter < HOUR) StartCounter++;
}
