#ifndef WORKFLOW
#define WORKFLOW

#define MODE_REGULATE 0

void SelfTest(void);
void SetPower(float value);
void LED_Control(char number, char status);
void LEDsBar(char count);
void ShowLED_Regulate(float _TempSetpoint, unsigned int _RawTemp);

#endif