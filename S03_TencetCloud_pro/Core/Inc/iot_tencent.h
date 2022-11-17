#ifndef __IOT_TENCENT_H_
#define __IOT_TENCET_H_
#include "main.h"


extern uint8_t TCMQTTRCVPUB[256];
extern uint8_t Sub_Topic[128];
extern uint8_t Sub_Data[128];

void Receive_Data_FromCloud_Data(int type, char *str);


#endif 





