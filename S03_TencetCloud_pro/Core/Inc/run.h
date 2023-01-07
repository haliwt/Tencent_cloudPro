#ifndef __RUN_H__
#define __RUN_H__
#include "main.h"

#define WIFI_TUYA      1  //
#define AI_ENABLE      1

typedef enum {                   /* enumeration for CProcess signals */
    kill =1,notkill,dry,notdry,ai,notai
}Signal;

typedef enum
{
  AIENABLE = 0,
  AIDISABLE = !AIENABLE
} AiStatus;

typedef enum
{
  WIFIENABLE =1,
  WIFIDISABLE = !WIFIENABLE

}WifiStatus;

typedef enum{

   POWER_ON =0x01,
   POWER_OFF,
   POWER_CONNECTOR_WIFI,
   UPDATE_TO_PANEL_DATA

   

}run_state_t;



typedef struct _RUN_T{
    
	uint8_t gPower_On;
	uint8_t wifi_gPower_On;
	uint8_t gPower_flag;
	uint8_t gDht11_flag;
	uint8_t gTemperature;

	uint8_t gDht11_humidity ;
	uint8_t gDht11_temperature;

    uint8_t gFan_continueRun;

	uint8_t RunComman_Lable;
	
  
    uint8_t decodeFlag;
	uint8_t sendtimes;
    uint8_t setup_timer_flag;
    uint8_t gmt_time_flag;
    uint8_t gTimer_send_0xaa;
      

      uint8_t  gAi;
      uint8_t  gPlasma;
      uint8_t  gDry;
	  uint8_t  gFan;
	  uint8_t  gUlransonic;
	  uint8_t  gModel;
      uint8_t Single_cmd ;
	  
	  uint8_t  set_temperature_value;
	  uint8_t  set_temperature_decade;
	  uint8_t  set_temperature_unit;
	  uint8_t  set_wind_speed_value;
      
	  uint8_t  wind_speed_decade;
	  uint8_t  wind_speed_unit;
      uint8_t wifi_config_net_lable;

	
    uint8_t gTimer_60s;
	uint8_t gTimer_wifi_1s;
	uint8_t gTimer_1s;

	uint8_t gFan_counter;

	
   
    
 }RUN_T;

//#define CProcessInit(me_) ((me_)->cmdCtr__ =0,(me_)->state__ = IDLE)
extern uint8_t ReceiveBuffer[1];
extern RUN_T run_t; 




void RunCommand_Order(void);

void Decode_RunCmd(void);



void Single_RunCmd(uint8_t sig);

void Single_Mode(void);

void Single_ReceiveCmd(uint8_t cmd);


#endif 


