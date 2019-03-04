/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"
#include "config.h"
#include "types.h"
#include "periphery.h"
#include "workflow.h"
#include "PIcontrol.h"
#include "flash.h"
#include <stdio.h>
#include <string.h> // memcpy
#include <stdlib.h> //realloc

FlagsTypeDef    Flags;
TempTypeDef     Temp;
PowerTypeDef    Power;
ButtonTypeDef   Button;
TimersTypeDef   Timers;
VoltagesTypeDef Voltages;

uint16_t    FLASH_STORAGE; //0x00 child-0ff, 0x10 child-on, 0x01 adult-off, 0x11 adult-on

RGB_TypeDef RGB_STATE;
RGB_TypeDef RGB_OFF;

int main(void){
  IO_Init();
  Timers_Init();
  Analog_Init();  
  ZeroSetPower(&Power);  
  InitFlags(&Flags);
  InitTemp(&Temp);
  InitTimers(&Timers);
  RGB_STATE = RGB_off;
  InitButton(&Button);
  MakeAllRight(&FLASH_STORAGE, &Flags, &Temp);
  StartConvertion();
  Flags.Buzzer = ENABLE;
  WaitUntil(&Timers, SYSTEM_READY);
  Flags.Buzzer = DISABLE;
  while(1){
    if(Flags.DeviceOn){
      //POWERED ON SECTION
      if(Flags.AcquireTemperature){
        //Temperature acquire every 1 sec
        ExecuteTemp(&Temp);
        if(Temp.Actual >= DANGER_TEMP) Flags.ErrorOverheating = ENABLE;
        //PI control      
        CalcPower(&Power, &Temp);
        SetPower(&Power);
        //LEDs
        TemperatureToRGB(&Temp, &RGB_STATE);
        RGB_Control(&RGB_STATE);
        if(!Button.InUse)ShowTempLED_Bar(&Temp);
        GetVoltages(&Voltages);
        Flags.AcquireTemperature = DISABLE;      
      }
      if(Button.ChangeTemp)LED_One((uint8_t)(Flags.SetTemp - MIN_TEMP) + 1);
    }else{
      //POWERED OFF SECTION
      ZeroSetPower(&Power);
      RGB_Control(RGB_off);
      LED_BarOff();
      Flags.Buzzer = DISABLE;
    }    
    ButtonProcess(&Button, &Flags, &Temp);
    CheckErrors(&Flags);
    if(Flags.Heater) RGB_Control(&RGB_STATE); else RGB_Control(&RGB_OFF);
  }
}



#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
    
  }
}
#endif