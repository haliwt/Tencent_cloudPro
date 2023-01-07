#ifndef __SUBSCRIPTION_H_
#define __SUBSCRIPTION_H_
#include "main.h"

typedef enum DISPLAY_STATE_T{
   
    WIFI_POWER_ON = 0x80,
    WIFI_POWER_OFF=0X81,
    WIFI_MODE_1=0X08,   //state ->normal works
    WIFI_MODE_2=0X18,   //state->sleeping works
    WIFI_KILL_ON=0x04,  //Anion(plasma)
    WIFI_KILL_OFF=0x14,
    WIFI_PTC_ON = 0x02, 
    WIFI_PTC_OFF = 0x12,
    WIFI_SONIC_ON = 0x01,       //ultrasonic
    WIFI_SONIC_OFF = 0x11,
    WIFI_WIND_SPEED = 0x90,
    WIFI_TEMPERATURE = 0xA0


}display_state_t;



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

