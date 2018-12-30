#define HEAT_ON          GPIO_SetBits(GPIOA, GPIO_Pin_8);
#define HEAT_OFF         GPIO_ResetBits(GPIOA, GPIO_Pin_8);

#define WIRE_RELEASE     GPIO_SetBits(GPIOA, GPIO_Pin_12);
#define WIRE_PULL        GPIO_ResetBits(GPIOA, GPIO_Pin_12);
#define WIRE_READ        GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12)

#define TEMP_RESOLITION   0.0625
#define HEAT_CYCLE        10000
#define TIME_CYCLE        10 

//#define RESET_ON        GPIO_SetBits(GPIOB, GPIO_Pin_7);
//#define RESET_OFF       GPIO_ResetBits(GPIOB, GPIO_Pin_7);
//
//#define DC_ON           GPIO_SetBits(GPIOB, GPIO_Pin_6);
//#define DC_OFF          GPIO_ResetBits(GPIOB, GPIO_Pin_6);