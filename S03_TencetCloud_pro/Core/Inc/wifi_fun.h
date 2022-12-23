#ifndef __WIFI_FUN_
#define __WIFI_FUN_
#include "main.h"


typedef enum{
  
   wifi_AI=0x08 ,wifi_notAI=0x18,wifi_kill=0x04,wifi_notkill=0x14,
   wifi_heat= 0x02,wifi_notheat=0x12


}wifi_mode;


typedef struct _WIFI_FUN{

    uint8_t wifi_power;
  
    uint8_t wifi_RunMode;

	uint8_t WifiMode;
	uint8_t wifi_cmd;
    uint8_t wifi_enable_flag;


	uint8_t wifi_ai;
	uint8_t wifi_itemAi;
	uint8_t wifi_dry;
	uint8_t wifi_kill;
	uint8_t wifi_counter; 
	uint8_t wifi_detect;
	uint8_t wifi_sensor;

	uint8_t real_hours;
	uint8_t real_minutes;
	uint8_t real_seconds;

	uint8_t setTimesValue;
	uint8_t dispTimesValue;
	uint8_t SetTemperatureValue;



    uint8_t getTime_flag;
	uint8_t gTimer_500ms;
	uint8_t gTimer_1s;
    
    uint8_t timer_wifi_send_cmd;
    uint8_t timer_wiifi_sensor;
	uint8_t wifi_timer_send_info;
    uint8_t getGreenTime;
    uint8_t gTimer_gmt;
	
	uint8_t getTime[7];
	


}WIFI_FUN;

extern WIFI_FUN   wifi_t;

extern void (*PowerOn)(void); //函数指针
extern void (*PowerOff)(void);
extern void (*Ai_Fun)(uint8_t sig);
extern void (*SetTimes)(void);
extern void (*SetTemperature)(void);

void PowerOn_Host(void (*poweronHandler)(void));
void PowerOff_Host(void (*poweroffHandler)(void));

void AI_Function_Host(void(*AIhandler)(uint8_t sig));

void SetTimeHost(void(*timesHandler)(void));
void SetTemperatureHost(void(*temperatureHandler)(void));

void Wifi_Mode(void);
void wifiDisplayTemperature_Humidity(void);
void wifiUpdate_Power_Status(uint8_t pv);
void wifiUpdate_Kill_Status(uint8_t kv);
void wifiUpdate_AI_Status(uint8_t av);
void wifiUpdate_Dry_Status(uint8_t dv);

void wifiUpdate_SetTimeValue(uint8_t tv);
void wifiUpdate_SetTemperatureValue(uint8_t temp);
void Wifi_RunMode(uint8_t cmd);




#endif 

