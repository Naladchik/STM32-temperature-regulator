#include "stm32f0xx.h"
#include "periphery.h"
#include "workflow.h"
#include "config.h"
#include "types.h"

void InitFlags(FlagsTypeDef* fl){
  fl->AcquireTemperature = DISABLE;
  fl->AdultMode = ENABLE;
  fl->Buzzer = DISABLE;
  fl->BeepCounter = 0;
  fl->Heater = DISABLE;
  fl->DeviceOn = ENABLE;
  fl->ErrorGlobal = DISABLE;
  fl->ErrorOverheating = DISABLE;
  fl->ErrorSafetySensor = DISABLE;
  fl->ErrorVoltageToHigh = DISABLE;
  fl->ErrorVoltageToLow = DISABLE;
  fl->Error_18b20 = DISABLE;
  fl->SetTemp = DEFAULT_TEMP;
}

void InitTemp(TempTypeDef* tm){
  tm->Actual = DEFAULT_TEMP;
  tm->Target = DEFAULT_TEMP;
  tm->e = 0;
  tm->Temp18b20 = (uint16_t)((float)DEFAULT_TEMP / TEMP_RESOLITION);
  tm->Integral = 0;
}

void InitTimers(TimersTypeDef* tm){
  tm->FromStart = 0;
}

void InitButton(ButtonTypeDef* bt){
  bt->InUse = DISABLE;
  bt->ChangeTemp = DISABLE;
  bt->ChangeOnOff = DISABLE;
  bt->ChangeAdult = DISABLE;  
  bt->PressedNow = DISABLE;
  bt->PressedWas = DISABLE;
  bt->JitterTimer = 0;
  bt->PressTimer = 0;
  bt->ActivityTimer = 0;
}

void ExecuteTemp(TempTypeDef* tm){
  tm->Temp18b20 = GetRawTemperature();
  tm->Actual = (float)(tm->Temp18b20) * TEMP_RESOLITION;        
  tm->e = tm->Target - tm->Actual;
  StartConvertion();
}

void SetPower(PowerTypeDef* pw){
  if(pw->Current < 0) pw->Current = 0;
  if(pw->Current > 1.0) pw->Current = 1.0;
  pw->HeaterOnTime = (unsigned int)(pw->Current * HEAT_CYCLE);
  pw->HeaterOffTime = HEAT_CYCLE - pw->HeaterOnTime;
}

void ZeroSetPower(PowerTypeDef* pw){
  pw->Current = 0;
  pw->HeaterOnTime = 0;
  pw->HeaterOffTime = HEAT_CYCLE;
  pw->HeaterStatus = DISABLE;
}

void ShowLED_Bar(int8_t length){
  for(uint8_t i = 1; i <= 8; i++){
    if(i <= length)LED_Control(i,ENABLE); else LED_Control(i,DISABLE);
  } 
}

void ShowTempLED_Bar(TempTypeDef* tm){
  uint8_t BarLength;
  uint8_t ActualInt;
  uint8_t MinimalInt;
  ActualInt = (uint8_t)(tm->Actual + 0.5); 
  MinimalInt = MIN_TEMP;
  if(ActualInt < MinimalInt) BarLength = 0; else{    
    BarLength = (uint8_t)(ActualInt - MinimalInt) + 1;    
  }  
  for(uint8_t i = 1; i <= 8; i++){
    if(i <= BarLength)LED_Control(i,ENABLE); else LED_Control(i,DISABLE);
  }  
}

void RGB_Control(RGB_TypeDef* rgb){
  if(*rgb == RGB_off){
    RED_OFF;
    GREEN_OFF;
    BLUE_OFF;
  }
  if(*rgb == RGB_red){
    RED_ON;
    GREEN_OFF;
    BLUE_OFF;
  }
  if(*rgb == RGB_white){
    RED_ON;
    GREEN_ON;
    BLUE_ON;
  }
}

void TemperatureToRGB(TempTypeDef* tm, RGB_TypeDef* rgb){
  if(*rgb == RGB_off) *rgb = RGB_red;
  if(*rgb == RGB_red){
    if(tm->e < 0.7) *rgb = RGB_white;          
  }else{
    if(*rgb == RGB_white){
      if(tm->e > 1.0) *rgb = RGB_red;
    }        
  }
}

void ButtonProcess(ButtonTypeDef* bt, FlagsTypeDef* fl, TempTypeDef* tm){  
  if((bt->PressedNow) && (!bt->PressedWas)){//Just pressed
    bt->PressTimer = 0;
    fl->BeepCounter = BEEP_LENGTH;
    fl->Buzzer = ENABLE;
  }
  
  if((!bt->PressedNow) && (bt->PressedWas)){ //Was pressed and just released
    if(bt->InUse){//was in use already
      if(bt->ChangeTemp){//Temp change case
        if(fl->AdultMode){
          if(fl->SetTemp < MAX_TEMP)fl->SetTemp++; else fl->SetTemp = MIN_TEMP;
        }else{
          if(fl->SetTemp < MAX_TEMP_CHILD)fl->SetTemp++; else fl->SetTemp = MIN_TEMP;
        }      
      }      
      if(bt->ChangeAdult){//Adult - Child case
      
      }
      
    }else{//was not in use
      if(bt->PressTimer < LONG_PRESS)bt->ChangeTemp = ENABLE;
      
    }
  }
  
  if((bt->PressedNow) && (bt->PressedWas)){//Was pressed and pressed now
    if((bt->PressTimer >= LONG_PRESS)&&(!bt->ChangeOnOff)){
      bt->ChangeOnOff = ENABLE;
      bt->PressTimer = 0;
    }
    if(bt->ChangeOnOff){//On - Off case
      if(fl->DeviceOn){
        ShowLED_Bar(TEMP_RANGE - (int8_t)(bt->PressTimer/BAR_PERIOD));
        if((bt->PressTimer/BAR_PERIOD) >= TEMP_RANGE) fl->DeviceOn = DISABLE;
      }else{
        ShowLED_Bar((int8_t)(bt->PressTimer/BAR_PERIOD));
        if((bt->PressTimer/BAR_PERIOD) >= TEMP_RANGE) fl->DeviceOn = ENABLE;
      }   
    }
  }
  
  if((!bt->PressedNow) && (!bt->PressedWas)){//nothing is pressed and was not pressed
    if((bt->ActivityTimer > ACTIVITY_DONE)&&(bt->InUse)){//return to main mode
      if(bt->ChangeTemp){
        tm->Target = fl->SetTemp;
        bt->ChangeTemp = DISABLE;
      }
      if(bt->ChangeOnOff)bt->ChangeOnOff = DISABLE;        
      if(bt->ChangeAdult)bt->ChangeAdult = DISABLE;      
      bt->InUse = DISABLE;
    }
  }
  
  
  
  if((bt->ChangeTemp)||(bt->ChangeOnOff)||(bt->ChangeAdult)) bt->InUse = ENABLE;
  bt->PressedWas = bt->PressedNow;
}

void WaitUntil(TimersTypeDef* tmrs, uint16_t limit){
  while(tmrs->FromStart < limit){}
}

void CheckErrors(FlagsTypeDef* err){
  if(err->ErrorHeater)       err->ErrorGlobal = ENABLE;
  if(err->ErrorOverheating)  err->ErrorGlobal = ENABLE;
  if(err->ErrorSafetySensor) err->ErrorGlobal = ENABLE;
  if(err->ErrorVoltageToLow) err->ErrorGlobal = ENABLE;
  if(err->Error_18b20)       err->ErrorGlobal = ENABLE;
  if(err->ErrorGlobal) err->DeviceOn = DISABLE;
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

void LED_One(char number){
  for(uint8_t i = 1; i <= 8; i++){
    if(i == number) LED_Control(i,ENABLE); else LED_Control(i,DISABLE);
  }
}

void LED_BarOff(void){
  for(uint8_t i = 1; i <= 8; i++){
    LED_Control(i,DISABLE);
  }
}

void GetVoltages(VoltagesTypeDef* vlt){
  vlt->Supply = readADC1(ADC_Channel_0);
  vlt->Current = readADC1(ADC_Channel_1);
  vlt->Safety = readADC1(ADC_Channel_1);
}

