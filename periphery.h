#ifndef PERIPHERY
#define PERIPHERY

//#define HEAT_ON          GPIO_ResetBits(GPIOA, GPIO_Pin_7);
#define HEAT_ON          GPIO_SetBits(GPIOA, GPIO_Pin_7);
#define HEAT_OFF         GPIO_ResetBits(GPIOA, GPIO_Pin_7);

#define RED_ON           GPIO_SetBits(GPIOA, GPIO_Pin_4);
#define RED_OFF          GPIO_ResetBits(GPIOA, GPIO_Pin_4);

#define GREEN_ON         GPIO_SetBits(GPIOA, GPIO_Pin_5);
#define GREEN_OFF        GPIO_ResetBits(GPIOA, GPIO_Pin_5);

#define BLUE_ON          GPIO_SetBits(GPIOA, GPIO_Pin_6);
#define BLUE_OFF         GPIO_ResetBits(GPIOA, GPIO_Pin_6);

#define BUZZ_ON          GPIO_SetBits(GPIOA, GPIO_Pin_11);
#define BUZZ_OFF         GPIO_ResetBits(GPIOA, GPIO_Pin_11);

#define LED_1_ON         GPIO_SetBits(GPIOA, GPIO_Pin_9);
#define LED_1_OFF        GPIO_ResetBits(GPIOA, GPIO_Pin_9);

#define LED_2_ON         GPIO_SetBits(GPIOA, GPIO_Pin_10);
#define LED_2_OFF        GPIO_ResetBits(GPIOA, GPIO_Pin_10);

#define LED_3_ON         GPIO_SetBits(GPIOA, GPIO_Pin_15);
#define LED_3_OFF        GPIO_ResetBits(GPIOA, GPIO_Pin_15);

#define LED_4_ON         GPIO_SetBits(GPIOB, GPIO_Pin_5);
#define LED_4_OFF        GPIO_ResetBits(GPIOB, GPIO_Pin_5);

#define LED_5_ON         GPIO_SetBits(GPIOB, GPIO_Pin_4);
#define LED_5_OFF        GPIO_ResetBits(GPIOB, GPIO_Pin_4);

#define LED_6_ON         GPIO_SetBits(GPIOB, GPIO_Pin_3);
#define LED_6_OFF        GPIO_ResetBits(GPIOB, GPIO_Pin_3);

#define LED_7_ON         GPIO_SetBits(GPIOB, GPIO_Pin_1);
#define LED_7_OFF        GPIO_ResetBits(GPIOB, GPIO_Pin_1);

#define LED_8_ON         GPIO_SetBits(GPIOB, GPIO_Pin_0);
#define LED_8_OFF        GPIO_ResetBits(GPIOB, GPIO_Pin_0);

#define WIRE_RELEASE     GPIO_SetBits(GPIOA, GPIO_Pin_12);
#define WIRE_PULL        GPIO_ResetBits(GPIOA, GPIO_Pin_12);
#define WIRE_READ        GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12)

//Init functions
void IO_Init(void);
void Timers_Init(void);
void Analog_Init(void);

//1-wire functions
void OneWireRelease(void);
void OneWirePull(void);
void OneWireInput(void);
void OneWireOutput(void);
unsigned char MasterResetPresence(void);
void WriteByte(unsigned char);
unsigned char ReadByte(void);
void StartConvertion(void);
unsigned int GetRawTemperature(void);
//Delay functions
void Delay_us(unsigned int);

void BuzzerControl(char status);
unsigned int readADC1(unsigned int channel);

#endif