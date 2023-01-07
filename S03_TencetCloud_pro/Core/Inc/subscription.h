#ifndef __SUBSCRIPTION_H_
#define __SUBSCRIPTION_H_
#include "main.h"

typedef enum _subSignal{ /* enumeration for CParser signals */
   subscrible_data,
   OPEN_SIG, 
   STATE_SIG, 
   PTC_SIG,
   SONIC_SIG,
   ANION_SIG,
   TEMP_SIG,
   FIND_SIG,
   HUM_SIG,
   NOWTEMP_SIG
}subSignal;


typedef enum _JSONTYPE {
   JSNONE = -1,
   JSSTRING = 0,
   JSOBJECT,
   JSARRAY,
   JSNUMBER,
   JSBOOLEAN,
   JSNULL,
   JSTYPEMAX
}JSONTYPE;

extern uint8_t TCMQTTRCVPUB[40];


void Receive_Data_FromCloud_Data(int type, char *str);
void Wifi_Rx_InputInfo_Handler(void);
void Parse_Rx_Cloud_Data(void);

void Subscriber_Data_FromCloud_Handler(void);
void Subscribe_Rx_Interrupt_Handler(void);
void Tencent_Cloud_Rx_Handler(void);
#endif 

