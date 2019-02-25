#define WAKE_UP           3
#define START_INTEGRAL    240
//#define RISE_TIME         (float)300
//#define COLD_DELTA        (float)2
//#define PREHEAT_TIME      20
//#define PREHEAT_K         (float)0.7

#define K_PID             (float)0.2
#define TI_PID            (float)120

#define TEMP_RESOLITION   (float)0.0625
#define DEFAULT_TEMP      46
#define MIN_TEMP          43
#define TEMP_RANGE        8
#define MAX_TEMP          MIN_TEMP + TEMP_RANGE - 1

#define HEAT_CYCLE        10000
#define TIM14PRESCALER    4779

#define TIM16PRESCALER    479
#define BUTTON_TIME       100
#define JITTER            5
#define SEC_CONST         1000

#define TIM17PRESCALER    120
#define BUZZER_TIME       50

#define HOUR 3600

#define F_ADDRESS_1       (uint32_t)0x08007C00
#define CHILD_ON          (uint16_t)0x0120 //random numbers not equal to 0000 or ffff
#define CHILD_OFF         (uint16_t)0x4509
#define ADULT_ON          (uint16_t)0x2782
#define ADULT_OFF         (uint16_t)0x6352
#define LAST_ADR          (uint32_t)(F_ADDRESS_1+0x3FE)

#define RGB_RED           0x02
#define RGB_WHITE         0x04

 
