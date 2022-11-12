#ifndef loTMSG_H
#define loTMSG_H
#include "ESP8266.h"
#include "cJSON.h"
#include "usart.h"

extern uint8_t TCMQTTRCVPUB[256];
extern uint8_t Sub_Topic[128];
extern uint8_t Sub_Data[128];

void loTMessageHandler(void);
void cJsonMessageHandler(uint8_t *cJsonDATA);
void Receive_Data_FromCloud_Data(int type, char *str);

#endif
