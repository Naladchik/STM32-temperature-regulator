#ifndef WORKFLOW
#define WORKFLOW
#include "types.h"

void InitFlags(FlagsTypeDef* fl);
void InitTemp(TempTypeDef* tm);
void InitTimers(TimersTypeDef* tm);
void InitButton(ButtonTypeDef* bt);
void ExecuteTemp(TempTypeDef* tm);
void SetPower(PowerTypeDef* pw);
void ZeroSetPower(PowerTypeDef* pw);
void ShowTempLED_Bar(TempTypeDef* tm);
void RGB_Control(RGB_TypeDef* rgb);
void TemperatureToRGB(TempTypeDef* tm, RGB_TypeDef* rgb);
void ButtonProcess(ButtonTypeDef* bt, FlagsTypeDef* fl, TempTypeDef* tm);
void WaitUntil(TimersTypeDef* tmrs, uint16_t limit);
void CheckErrors(FlagsTypeDef* err);

void LED_Control(char number, char status);
void LED_One(char number);
void ShowLED_Bar(int8_t length);
void LED_BarOff(void);
void GetVoltages(VoltagesTypeDef* vlt);

#endif