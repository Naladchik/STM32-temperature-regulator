#include "stm32f0xx.h"
#include "config.h"
#include "types.h"
#include "flash.h"

uint8_t IsFlashClear(){
  uint16_t val;
  val = *(uint16_t*)F_ADDRESS_1;
  if(val == 0xffff)return(1); else return(0);
}

void ClearFlash(void){
  FLASH_Unlock();
  FLASH_ErasePage(F_ADDRESS_1);
  FLASH_Lock();
}

void ReadSettings(uint16_t* storage){
  *storage = *(uint16_t*)F_ADDRESS_1;
}

void StoreSettings(uint16_t* storage){
  FLASH_Unlock();
  while(FLASH_GetFlagStatus(FLASH_FLAG_BSY) == SET);
  FLASH_ErasePage(F_ADDRESS_1);
  FLASH_ProgramHalfWord(F_ADDRESS_1, *storage);
  FLASH_Lock();
}

uint8_t CheckStorage(uint16_t* storage){
  uint8_t ret = 1;
  uint16_t buff1, buff2;
  buff1 = *storage & 0xff00;
  buff1 >>= 8;
  if((buff1 != 0x00)&&(buff1 != 0x01)&&(buff1 != 0x10)&&(buff1 != 0x11))ret = 0;
  buff2 = *storage & 0x00ff;
  if((buff2 < MIN_TEMP )||(buff2 > MAX_TEMP))ret = 0;  
  if(((buff1&0x01) == 0x00) && (buff2 > MAX_TEMP_CHILD))ret = 0;
  return(ret);
}

void DefaultStorage(uint16_t* storage){
  *storage = 0x0000;
  *storage |= DEFAULT_STATE;
  *storage |= DEFAULT_TEMP;
}

void ExtractFromStorage(uint16_t* storage, FlagsTypeDef* fl, TempTypeDef* tm){
  if((*storage & 0x1000) == 0)fl->DeviceOn = DISABLE; else fl->DeviceOn = ENABLE;
  if((*storage & 0x0100) == 0)fl->AdultMode = DISABLE; else fl->AdultMode = ENABLE;
  tm->Target = (float)(*storage & 0x00ff);  
}

void PackToStorage(FlagsTypeDef* fl, TempTypeDef* tm, uint16_t* storage){
  *storage = 0x0000;
  if(fl->DeviceOn) *storage |= 0x1000;
  if(fl->AdultMode) *storage |= 0x0100;
  *storage |= (uint16_t)tm->Target;
}

void MakeAllRight(uint16_t* storage, FlagsTypeDef* fl, TempTypeDef* tm){
  if(IsFlashClear()){
    DefaultStorage(storage);
    StoreSettings(storage);
  }else{
    ReadSettings(storage);
    if(CheckStorage(storage)) ExtractFromStorage(storage, fl, tm); else{
      DefaultStorage(storage);
      ClearFlash();
      StoreSettings(storage);      
    }
  }
  ExtractFromStorage(storage, fl, tm);
  fl->SetTemp = (uint16_t)tm->Target;
}
