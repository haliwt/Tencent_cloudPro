#ifndef __SPECIAL_POWER_H_
#define __SPECIAL_POWER_H_
#include "main.h"



void SetPowerOn_ForDoing(void);
void SetPowerOff_ForDoing(void);

extern void (*Single_Usart_ReceiveData)(uint8_t cmd);

void Single_Usart_RxData(void(*rxHandler)(uint8_t dat));


#endif 
