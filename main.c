/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"
#include "config.h"
#include "periphery.h"
#include "workflow.h"
#include "PIcontrol.h"
#include <stdio.h>
#include <string.h> // memcpy
#include <stdlib.h> //realloc

extern char CommForPIcontrol;
extern char CommForTempConv;
extern char PreHeat;

extern int RawTemp;
extern float eTemp;
extern char TempSetpoint;
float ActualTemperature;
float SetTemperature;

uint8_t RGB_status = 0;

char OldButtonState = 0;
char CurrentButtonState = 0;
uint16_t ERROR_REG = 0;

uint8_t OnOffStatus = 1;

int main(void){
  IO_Init();
  Timers_Init();
  Analog_Init();
  SetPower(0.00);
  SetTemperature = (float)TempSetpoint;
  while(1){
    if(OnOffStatus == 1){
      //POWERED ON SECTION
      //Temperature acquire every 1 sec
      if(CommForTempConv != 0){
        RawTemp = GetRawTemperature();
        ActualTemperature = (float)RawTemp * TEMP_RESOLITION;        
        eTemp = SetTemperature - ActualTemperature;
        StartConvertion();
        CommForPIcontrol = 1;
        CommForTempConv = 0;
      }      
      if(CommForPIcontrol != 0){
        UpdatePower();        
        CommForPIcontrol = 0;
        if(RGB_status == RGB_RED){
          if((SetTemperature - ActualTemperature) < 0.5) RGB_status = RGB_WHITE;
          RED_ON;
          GREEN_OFF;
          BLUE_OFF;
        }else{
          if(RGB_status == RGB_WHITE){
            if((SetTemperature - ActualTemperature) > 1.0) RGB_status = RGB_RED;
            RED_ON;
            GREEN_ON;
            BLUE_ON;
          }else{}        
        }
      }
     // LEDsControl(bbb);
      
    }else{
      //POWERED OFF SECTION
      
      
    }
    
    //BUTTON CONTROL
    if(CurrentButtonState != 0){
      //Button pressed
      if(OldButtonState == 0){
        //Button pressed and released
        
      }
    }
    OldButtonState = CurrentButtonState;
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