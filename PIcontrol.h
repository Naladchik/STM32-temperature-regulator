#ifndef PICONTROL
#define PICONTROL

void Calc_eTemp(void);
void CalcPower(PowerTypeDef* pw, TempTypeDef* tm);
void HeatUp(void);

#endif