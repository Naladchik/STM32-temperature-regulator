/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"
#include "config.h"
#include "periphery.h"
#include "workflow.h"
#include "PIcontrol.h"

extern char CommForPIcontrol;
extern char CommForTempConv;
extern char PreHeat;

extern int RawTemp;
extern char TempSetpoint;

char Blink = 0;
char OldButtonState = 0;
char CurrentButtonState = 0;



int main(void)
{    
    IO_Init();
    Timers_Init();
    SelfTest();
    SetPower(0.00);
    while(1){
      //Temperature acquire every 1 sec
      if(CommForTempConv != 0){
        RawTemp = GetRawTemperature();        
        StartConvertion();
        Calc_eTemp();
        CommForPIcontrol = 1;
        CommForTempConv = 0;
      }
      
      if(CommForPIcontrol != 0){
        if(PreHeat != 0) HeatUp(); else UpdatePower();
        CommForPIcontrol = 0;
      }
      
      //Indicating LED
      ShowLED_Regulate(TempSetpoint, RawTemp);
      //UI realization
      CurrentButtonState = Button();
      if(CurrentButtonState != 0){
        if(OldButtonState == 0){
          LED_Control(TempSetpoint - MIN_TEMP + 1, DISABLE);
          Blink = 1;
          if(TempSetpoint == MAX_TEMP) TempSetpoint = MIN_TEMP; else TempSetpoint++;
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