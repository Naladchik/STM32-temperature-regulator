/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"
#include "config.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
GPIO_InitTypeDef GPIO_InitStruct;
GPIO_InitTypeDef GPIO_1wire_InitStruct;
TIM_TimeBaseInitTypeDef TIM3_TimeBaseInitStruct;
TIM_TimeBaseInitTypeDef TIM14_TimeBaseInitStruct;
unsigned char a[9]; //1-wire read buffer
double TempSetpoint = 39.0; //setpoint temperature in Celsium degree
unsigned int TempSPValue; //setpoint in 0.0625 parts of Celsium degree
unsigned int TempActValue; //setpoint in 0.0625 parts of Celsium degree
unsigned int HeatRatio = 10; // ratio to HEAT_CYCLE
double e = 0; //difference for PID controller
double PID_buf;
unsigned char TimeCounter = 0;
unsigned int TIM14_period;


/* Private function prototypes -----------------------------------------------*/
void MySystemInit(void);
void OneWireRelease(void);
void OneWirePull(void);
void OneWireInput(void);
void OneWireOutput(void);
unsigned char MasterResetPresence(void);
void WriteByte(unsigned char);
unsigned char ReadByte(void);
void Delay_us(unsigned int);
void StartConvertion(void);
unsigned int GetTemperature(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f0xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f0xx.c file
     */
  MySystemInit();
  
  //start pause
  Delay_us(50000);
    
  /* Infinite loop */
  while (1)
  {
    if(HeatRatio > HEAT_CYCLE) HeatRatio = HEAT_CYCLE;
    if(HeatRatio > 0) HEAT_ON;
    TIM14_period = HeatRatio;
    if(TIM14_period < 10) TIM14_period = 10;
    TIM14_TimeBaseInitStruct.TIM_Period = TIM14_period;
    TIM_TimeBaseInit(TIM14,&TIM14_TimeBaseInitStruct);
    TIM_ClearFlag(TIM14, TIM_FLAG_Update);
    while(TIM_GetFlagStatus(TIM14, TIM_FLAG_Update) == RESET){
      
    }
    HEAT_OFF;
    TIM14_period = HEAT_CYCLE - HeatRatio;
    if(TIM14_period < 10) TIM14_period = 10;
    TIM14_TimeBaseInitStruct.TIM_Period = TIM14_period;
    TIM_TimeBaseInit(TIM14,&TIM14_TimeBaseInitStruct);
    TIM_ClearFlag(TIM14, TIM_FLAG_Update);
    while(TIM_GetFlagStatus(TIM14, TIM_FLAG_Update) == RESET){
      
    }
    //we are here every 100 ms    
    TimeCounter ++;
    if(TimeCounter == TIME_CYCLE){
      StartConvertion();
      TimeCounter = 0;
    }
    if(TimeCounter == TIME_CYCLE - 1){
      TempActValue = GetTemperature();
      TempSPValue = (unsigned int)(TempSetpoint / TEMP_RESOLITION);
      e = (double)TempSPValue - (double)TempActValue;
      
    }
    
    PID_buf = e * 1000;
    PID_buf += (double)HEAT_CYCLE/2;
    if(PID_buf < 0) HeatRatio = 0; else HeatRatio = (unsigned int)PID_buf;
    
    
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

/** 
  * @}
  */
void MySystemInit(void){
  /*GPIO section*/
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB ,ENABLE);
  //Heat output initialization
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_Level_3;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA,&GPIO_InitStruct);
  HEAT_OFF;
  //1-wire initialisation
  GPIO_1wire_InitStruct.GPIO_Pin = GPIO_Pin_12;
  GPIO_1wire_InitStruct.GPIO_Mode = GPIO_Mode_IN;
  GPIO_1wire_InitStruct.GPIO_Speed = GPIO_Speed_Level_3;
  GPIO_1wire_InitStruct.GPIO_OType = GPIO_OType_OD;
  GPIO_1wire_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA,&GPIO_1wire_InitStruct);
  WIRE_RELEASE;
  //Timer 3 for 1-wire
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 ,ENABLE);
  TIM_DeInit(TIM3);
  TIM3_TimeBaseInitStruct.TIM_Prescaler = 0x002F; //47+1 division to obtain 1 us
  TIM3_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Down;
  TIM3_TimeBaseInitStruct.TIM_Period = 0xFFFF;
  TIM3_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseInit(TIM3,&TIM3_TimeBaseInitStruct);
  TIM_Cmd(TIM3, ENABLE);
  
  //Timer 14 for heat drive
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14 ,ENABLE);
  TIM_DeInit(TIM14);
  TIM14_TimeBaseInitStruct.TIM_Prescaler = 0x01DF; //479+1 division to obtain 10 us
  TIM14_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Down;
  TIM14_TimeBaseInitStruct.TIM_Period = 0xFFFF;
  TIM14_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV2;
  TIM_TimeBaseInit(TIM14,&TIM14_TimeBaseInitStruct);
  TIM_Cmd(TIM14, ENABLE);
  
}



/**
  * @}
  */
void OneWireRelease(void){
  WIRE_RELEASE;
}

/**
  * @}
  */
void OneWirePull(void){
  WIRE_PULL;
}

/**
  * @}
  */
void OneWireInput(void){
  //WIRE_RELEASE;
  GPIO_1wire_InitStruct.GPIO_Pin = GPIO_Pin_12;
  GPIO_1wire_InitStruct.GPIO_Mode = GPIO_Mode_IN;
  GPIO_1wire_InitStruct.GPIO_Speed = GPIO_Speed_Level_3;
  GPIO_1wire_InitStruct.GPIO_OType = GPIO_OType_OD;
  GPIO_1wire_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA,&GPIO_1wire_InitStruct);  
}

/**
  * @}
  */
void OneWireOutput(void){
  GPIO_1wire_InitStruct.GPIO_Pin = GPIO_Pin_12;
  GPIO_1wire_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_1wire_InitStruct.GPIO_Speed = GPIO_Speed_Level_3;
  GPIO_1wire_InitStruct.GPIO_OType = GPIO_OType_OD;
  GPIO_1wire_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA,&GPIO_1wire_InitStruct);
  //WIRE_RELEASE;
}

/**
  * @}
  */
unsigned char MasterResetPresence(void){
  unsigned char present1, present2;
  present1 = WIRE_READ;
  OneWireOutput();
  OneWirePull();
  Delay_us(750);
  OneWireRelease();
  OneWireInput();
  Delay_us(70);
  present2 = WIRE_READ;
  if (present1 == 1 && present2 == 0) present1 = 1; else present1 = 0;
  
  //wait for slave release
  TIM3_TimeBaseInitStruct.TIM_Period = 750;
  TIM_TimeBaseInit(TIM3,&TIM3_TimeBaseInitStruct);
  TIM_ClearFlag(TIM3, TIM_FLAG_Update);
  while(TIM_GetFlagStatus(TIM3, TIM_FLAG_Update) == RESET){
    present2 = WIRE_READ;
    if(present2 != 0) break;
  }
  
  Delay_us(600);

  return(present1);
}

/**
  * @}
  */
void WriteByte(unsigned char b){
  unsigned char i;
  for(i = 0; i < 8; i++){
    OneWireOutput();    
    if( b & 0x80 == 0x80 ){
      //write 1 time slot
      OneWirePull();
      Delay_us(7);
      OneWireRelease();
      Delay_us(60);
    }else{
      //write 0 time slot
      OneWirePull();
      Delay_us(70);
      OneWireRelease();
      Delay_us(10);
    }
    b >>= 1;  
  }
  OneWireInput();
}

/**
  * @}
  */
unsigned char ReadByte(void){
  unsigned char b,i;
  b = 0;
  for(i = 0; i < 8; i++){
    OneWireOutput();      
    //read time slot
    OneWirePull();
    Delay_us(3);
    OneWireRelease();
    OneWireInput();
    Delay_us(5);
    if(WIRE_READ != 0) b |= 0x80;    
    Delay_us(60);    
    if(i != 7) b >>= 1;  
  }
  return(b);
}

/**
  * @}
  */
void Delay_us(unsigned int us){
  TIM3_TimeBaseInitStruct.TIM_Period = us;
  TIM_TimeBaseInit(TIM3,&TIM3_TimeBaseInitStruct);
  TIM_ClearFlag(TIM3, TIM_FLAG_Update);
  while(TIM_GetFlagStatus(TIM3, TIM_FLAG_Update) == RESET){}  
}

/**
  * @}
  */
void StartConvertion(void){
  if (MasterResetPresence() != 0){
    WriteByte(0xCC);
    WriteByte(0x44);      
  }
}

/**
  * @}
  */
unsigned int GetTemperature(void){
  unsigned int temp;
  if (MasterResetPresence() != 0){
    WriteByte(0xCC);
    WriteByte(0xBE);
    for(unsigned char i = 0; i<9; i++){
      a[i] = ReadByte();
    }
    temp = a[1];    
    temp <<= 8;
    temp |= a[0];
  }else{
    temp = 0x07D0;
  }
  return(temp);
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
