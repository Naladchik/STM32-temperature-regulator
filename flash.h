#ifndef FLASH_H
#define FLASH_H

#include "types.h"

uint8_t IsFlashClear();
void ClearFlash(void);
void ReadSettings(uint16_t* storage);
void StoreSettings(uint16_t* storage);
uint8_t CheckStorage(uint16_t* storage);
void DefaultStorage(uint16_t* storage);
void ExtractFromStorage(uint16_t* storage, FlagsTypeDef* fl, TempTypeDef* tm);
void PackToStorage(FlagsTypeDef* fl, TempTypeDef* tm, uint16_t* storage);
void MakeAllRight(uint16_t* storage, FlagsTypeDef* fl, TempTypeDef* tm);

#endif