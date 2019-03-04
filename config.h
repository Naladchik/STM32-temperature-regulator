#ifndef CONFIG
#define CONFIG

#define SYSTEM_READY      1000

#define DEFAULT_STATE    0x1100 //0x00 child-0ff, 0x10 child-on, 0x01 adult-off, 0x11 adult-on

#define K_PID             (float)0.2 
#define TI_PID            (float)120 

#define BASE_TEMP         35
#define TEMP_CORRECTION   2
#define MIN_TEMP          (BASE_TEMP + TEMP_CORRECTION)
#define DEFAULT_TEMP      (MIN_TEMP + 3)
#define TEMP_RANGE        8
#define MAX_TEMP          (MIN_TEMP + TEMP_RANGE - 1)
#define MAX_TEMP_CHILD    (MAX_TEMP - 2)
#define DANGER_TEMP       (float)60.0
#define TEMP_RESOLITION   (float)0.0625

#define HEAT_CYCLE        10000
#define TIM14PRESCALER    4779

#define JITTER            5
#define TIM16PRESCALER    479
#define BUTTON_PERIOD     100
#define LONG_PRESS        1000
#define ACTIVITY_DONE     5000

#define TIM17PRESCALER    120
#define BUZZER_TIME       50
#define BEEP_LENGTH       500

#define BUTTON_SPEED      200
#define BAR_PERIOD        (uint16_t)200

#define F_ADDRESS_1       (uint32_t)0x08007C00

#define MINUTE            60000

#endif

 
