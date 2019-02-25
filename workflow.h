#ifndef WORKFLOW
#define WORKFLOW

#define MODE_REGULATE 0

void SetPower(float value);
void LED_Control(char number, char status);
void LEDsControl(uint8_t byte);
void ShowTempBar(uint8_t value);

#endif