#include "stm32f0xx.h"
#include "periphery.h"
#include "workflow.h"
#include "config.h"
#include "types.h"

extern uint8_t RGB_status;
extern PowerTypeDef    Power;

unsigned int StartCounter = 0;

extern TempTypeDef     Temp;

void CalcPower(PowerTypeDef* pw, TempTypeDef* tm){
  pw->Current = K_PID * (Temp.e + tm->Integral/TI_PID);
  if((pw->Current < 1.0) && (pw->Current > 0.0)) tm->Integral += Temp.e;  
}
