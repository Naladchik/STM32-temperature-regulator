#include "stm32f0xx.h"
#include "periphery.h"
#include "config.h"

unsigned char wire_buffer[9]; //1-wire read buffer
unsigned int TIM14_period;

GPIO_InitTypeDef GPIO_InitStruct;
TIM_TimeBaseInitTypeDef TIM14_TimeBaseInitStruct;
TIM_TimeBaseInitTypeDef TIM16_TimeBaseInitStruct;
TIM_TimeBaseInitTypeDef TIM17_TimeBaseInitStruct;
TIM_TimeBaseInitTypeDef TIM3_TimeBaseInitStruct;
GPIO_InitTypeDef GPIO_1wire_InitStruct;


void IO_Init(void){
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB ,ENABLE);  
  GPIO_InitTypeDef GPIO_InitStruct;
  //RGB, Heat output, buzzer initialization
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_11;
  //LED 206 - 208
  GPIO_InitStruct.GPIO_Pin |= GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_15;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_Level_3;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA,&GPIO_InitStruct);
  HEAT_OFF;
  BUZZ_OFF;
  RED_OFF;
  GREEN_OFF;
  BLUE_OFF;
  LED_6_OFF;
  LED_7_OFF;
  LED_8_OFF;
  //LED 201 - 205
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_Level_3;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB,&GPIO_InitStruct);
  LED_1_OFF;
  LED_2_OFF;
  LED_3_OFF;
  LED_4_OFF;
  LED_5_OFF;
  //1-wire initialisation
  GPIO_1wire_InitStruct.GPIO_Pin = GPIO_Pin_12;
  GPIO_1wire_InitStruct.GPIO_Mode = GPIO_Mode_IN;
  GPIO_1wire_InitStruct.GPIO_Speed = GPIO_Speed_Level_3;
  GPIO_1wire_InitStruct.GPIO_OType = GPIO_OType_OD;
  GPIO_1wire_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA,&GPIO_1wire_InitStruct);
  WIRE_RELEASE;
  //Button
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_Level_3;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA,&GPIO_InitStruct);
}

void Timers_Init(void){
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
  //Supposed to have 1 sec period with 0.0001 (100 us) resolution
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14 ,ENABLE);
  TIM_DeInit(TIM14);
  TIM14_TimeBaseInitStruct.TIM_Prescaler = TIM14PRESCALER; //4799+1 division to obtain 100 us
  TIM14_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Down;
  TIM14_TimeBaseInitStruct.TIM_Period = HEAT_CYCLE;
  TIM14_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV2;
  TIM_TimeBaseInit(TIM14,&TIM14_TimeBaseInitStruct);
  TIM_Cmd(TIM14, ENABLE);
  TIM_ITConfig(TIM14,TIM_IT_Update,ENABLE);
  NVIC_EnableIRQ (TIM14_IRQn);
  
  //Timer 16 for button reading
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM16 ,ENABLE);
  TIM_DeInit(TIM16);
  TIM16_TimeBaseInitStruct.TIM_Prescaler = TIM16PRESCALER; //479+1 division to obtain 10 us
  TIM16_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Down;
  TIM16_TimeBaseInitStruct.TIM_Period = BUTTON_TIME;
  TIM16_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV2;
  TIM_TimeBaseInit(TIM16,&TIM16_TimeBaseInitStruct);
  TIM_Cmd(TIM16, ENABLE);
  TIM_ITConfig(TIM16,TIM_IT_Update,ENABLE);
  NVIC_EnableIRQ (TIM16_IRQn);
  
  
  //Timer 17 Buzzer
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM17 ,ENABLE);
  TIM_DeInit(TIM17);
  TIM17_TimeBaseInitStruct.TIM_Prescaler = TIM17PRESCALER;
  TIM17_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Down;
  TIM17_TimeBaseInitStruct.TIM_Period = BUZZER_TIME;
  TIM17_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV2;
  TIM_TimeBaseInit(TIM17,&TIM17_TimeBaseInitStruct);
  TIM_Cmd(TIM17, ENABLE);
  TIM_ITConfig(TIM17,TIM_IT_Update,ENABLE);
  NVIC_EnableIRQ (TIM17_IRQn);
  
  __enable_irq();
}


void Analog_Init(void){
  GPIO_InitTypeDef GPIO_InitStruct;
  ADC_InitTypeDef ADC_InitStructure;
  
  //GPIOA analog inputs
    GPIO_InitStruct.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2;
    GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AN;
    GPIO_Init(GPIOA,&GPIO_InitStruct);  
    RCC_ADCCLKConfig (RCC_ADCCLK_PCLK_Div4);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConvEdge_None;
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
    ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Upward;
    ADC_Init(ADC1, &ADC_InitStructure);
    ADC_Cmd (ADC1,DISABLE);
    if( ADC_GetCalibrationFactor(ADC1) == 0){}
    ADC_Cmd (ADC1,ENABLE);
//    ADC_SoftwareStartConvCmd(ADC1, ENABLE); // start conversion (will be endless as we are in continuous mode)
    if(readADC1(ADC_Channel_0) == 0){}
    if(readADC1(ADC_Channel_1) == 0){}
    if(readADC1(ADC_Channel_2) == 0){}
    if(readADC1(ADC_Channel_0) == 0){}
    if(readADC1(ADC_Channel_1) == 0){}
    if(readADC1(ADC_Channel_2) == 0){}  
}


unsigned int readADC1(unsigned int channel){
  ADC_ChannelConfig(ADC1, channel, ADC_SampleTime_239_5Cycles);
  ADC_StartOfConversion(ADC1);
  for(uint16_t i=0; i<10000; i++){
      if(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) != RESET) break;
      asm volatile(" nop " "\n\t");
    }
  return ADC_GetConversionValue(ADC1);
}



unsigned char Button(void){
  extern char ButtonStatus;
  return(ButtonStatus);
}


void OneWireInput(void){
  GPIO_1wire_InitStruct.GPIO_Pin = GPIO_Pin_12;
  GPIO_1wire_InitStruct.GPIO_Mode = GPIO_Mode_IN;
  GPIO_1wire_InitStruct.GPIO_Speed = GPIO_Speed_Level_3;
  GPIO_1wire_InitStruct.GPIO_OType = GPIO_OType_OD;
  GPIO_1wire_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA,&GPIO_1wire_InitStruct);  
}

void OneWireOutput(void){
  GPIO_1wire_InitStruct.GPIO_Pin = GPIO_Pin_12;
  GPIO_1wire_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_1wire_InitStruct.GPIO_Speed = GPIO_Speed_Level_3;
  GPIO_1wire_InitStruct.GPIO_OType = GPIO_OType_OD;
  GPIO_1wire_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA,&GPIO_1wire_InitStruct);
}


unsigned char MasterResetPresence(void){
  unsigned char present1, present2;
  present1 = WIRE_READ;
  OneWireOutput();
  WIRE_PULL;;
  Delay_us(750);
  WIRE_RELEASE;;
  OneWireInput();
  Delay_us(70);
  present2 = WIRE_READ;
  if (present1 == 1 && present2 == 0) present1 = 1; else present1 = 0;
  
  //wait for slave release
  TIM_TimeBaseInitTypeDef TIM3_TimeBaseInitStruct;
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
      WIRE_PULL;
      Delay_us(7);
      WIRE_RELEASE;;
      Delay_us(60);
    }else{
      //write 0 time slot
      WIRE_PULL;
      Delay_us(70);
      WIRE_RELEASE;;
      Delay_us(10);
    }
    b >>= 1;  
  }
  OneWireInput();
}


unsigned char ReadByte(void){
  unsigned char b,i;
  b = 0;
  for(i = 0; i < 8; i++){
    OneWireOutput();      
    //read time slot
    WIRE_PULL;
    Delay_us(3);
    WIRE_RELEASE;;
    OneWireInput();
    Delay_us(5);
    if(WIRE_READ != 0) b |= 0x80;    
    Delay_us(60);    
    if(i != 7) b >>= 1;  
  }
  return(b);
}

void Delay_us(unsigned int us){
  if(us>0){
    TIM3_TimeBaseInitStruct.TIM_Period = us;
    TIM_TimeBaseInit(TIM3,&TIM3_TimeBaseInitStruct);
    TIM_ClearFlag(TIM3, TIM_FLAG_Update);
    while(TIM_GetFlagStatus(TIM3, TIM_FLAG_Update) == RESET){}
  }
}

void Delay_ms(unsigned int ms){
  for(unsigned int i = 0; i < ms; i++){
    Delay_us(1000);
  }
}


void StartConvertion(void){
  TIM_ITConfig(TIM16,TIM_IT_Update,DISABLE);
  TIM_ITConfig(TIM17,TIM_IT_Update,DISABLE);
  if (MasterResetPresence() != 0){
    WriteByte(0xCC);
    WriteByte(0x44);      
  }
  TIM_ITConfig(TIM16,TIM_IT_Update,ENABLE);
  TIM_ITConfig(TIM17,TIM_IT_Update,ENABLE);
}


unsigned int GetRawTemperature(void){
  TIM_ITConfig(TIM16,TIM_IT_Update,DISABLE);
  TIM_ITConfig(TIM17,TIM_IT_Update,DISABLE);
  unsigned int temp;
  if (MasterResetPresence() != 0){
    WriteByte(0xCC);
    WriteByte(0xBE);
    for(unsigned char i = 0; i<9; i++){
      wire_buffer[i] = ReadByte();
    }
    temp = wire_buffer[1];    
    temp <<= 8;
    temp |= wire_buffer[0];
  }else{
    temp = 0xffff;
  }
  TIM_ITConfig(TIM16,TIM_IT_Update,ENABLE);
  TIM_ITConfig(TIM17,TIM_IT_Update,ENABLE);
  return(temp);
}

extern char BuzzerOnOff;
void BuzzerControl(char status){
  if(status == 0)BuzzerOnOff = 0; else BuzzerOnOff = 1;
}