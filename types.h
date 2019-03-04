#ifndef TYPES
#define TYPES
#include "stm32f0xx.h"

//FLAGS
typedef struct{
  uint8_t DeviceOn:1;
  uint8_t AdultMode:1;  
  uint8_t AcquireTemperature:1;
  uint16_t SetTemp; //temporary variable for target temperature
  uint8_t Buzzer:1;
  uint16_t BeepCounter;
  uint8_t Heater:1;
  uint8_t ErrorGlobal:1;
  uint8_t ErrorOverheating:1;
  uint8_t ErrorVoltageToHigh:1;
  uint8_t ErrorVoltageToLow:1;
  uint8_t Error_18b20:1;
  uint8_t ErrorSafetySensor:1;
  uint8_t ErrorHeater:1;  
}FlagsTypeDef;

//TEMP
typedef struct{
  uint16_t Temp18b20;
  float Actual;
  float Target;
  float e;
  float Integral;
}TempTypeDef;

//POWER
typedef struct{
  float Current;
  uint16_t HeaterOnTime;
  uint16_t HeaterOffTime;
  uint8_t HeaterStatus:1;
}PowerTypeDef;

//TIMERS
typedef struct{
  uint16_t FromStart;
}TimersTypeDef;

//RGB
typedef enum{
  RGB_off = 0,
  RGB_red = 1,
  RGB_white = 2
}RGB_TypeDef;

//BUTTON
typedef struct{
  uint8_t InUse:1;
  uint8_t ChangeTemp:1;
  uint8_t ChangeOnOff:1;
  uint8_t ChangeAdult:1;
  uint8_t PressedNow:1;
  uint8_t PressedWas:1;
  uint16_t JitterTimer;
  uint16_t PressTimer;  
  uint16_t ActivityTimer;
}ButtonTypeDef;

//VOLTAGES
typedef struct{
  uint16_t Supply;
  uint16_t Current;
  uint16_t Safety;
}VoltagesTypeDef;

#endif