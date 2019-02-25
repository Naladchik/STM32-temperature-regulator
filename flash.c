#include "stm32f0xx.h"
#include "config.h"


uint8_t StoreStatus(uint16_t st){
  FLASH_Unlock();
  while(FLASH_GetFlagStatus(FLASH_FLAG_BSY) == SET);
  FLASH_ErasePage(F_ADDRESS_1);
  FLASH_ProgramHalfWord(F_ADDRESS_1, 56);
  FLASH_Lock();
  if(st == 0) return(0); else return(1);
}

uint16_t ReStoreStatus(void){
  uint16_t val;
  val = *(uint16_t*)F_ADDRESS_1;
  return(val);
}

