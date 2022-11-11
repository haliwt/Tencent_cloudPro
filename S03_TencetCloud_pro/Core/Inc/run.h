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



typedef struct _RUN_T{
    
	uint8_t gPower_On;
	uint8_t gPower_flag;
	uint8_t gDht11_flag;
	uint8_t gTemperature;

	uint8_t gDht11_humidity ;
	uint8_t gDht11_temperature;

    uint8_t gFan_continueRun;
	uint8_t SingleMode;
	uint8_t Single_cmd;
  
    uint8_t decodeFlag;
	uint8_t sendtimes;
    uint8_t setup_timer_flag;
    uint8_t gmt_time_flag;
    uint8_t gTimer_send_0xaa;
      

      uint8_t  gAi;
      uint8_t  gPlasma;
      uint8_t  gDry;
	  uint8_t  gFan;

	uint8_t kill_key;
	uint8_t kill_key_off;

	uint8_t dry_key;
	uint8_t dry_key_off;

	uint8_t ai_key;
	uint8_t ai_key_off;
	
	uint8_t wifi_key;
	uint8_t wifi_key_off;
    uint8_t wifi_init_flag;
    uint8_t wifi_cwsap_flag;
	uint8_t wifi_cwmode_flag;
	

	uint8_t  globe_sub_flag;
	uint8_t set_temperature_on;
	uint8_t set_temperature_off;



	uint8_t gTimer_60s;
	uint8_t gTimer_wifi_1s;

	uint8_t gFan_counter;

	uint8_t globe_setPriority;
   
    
    
	

	
}RUN_T;

//#define CProcessInit(me_) ((me_)->cmdCtr__ =0,(me_)->state__ = IDLE)
extern uint8_t ReceiveBuffer[1];
extern RUN_T run_t; 



void RunCommand_Mode(uint8_t sig);
void RunCommand_Order(void);

void Decode_RunCmd(void);
void Initial_Ref(void);

void AI_Function(uint8_t sig);
void Single_ReceiveCmd(uint8_t cmd);

void Wifi_ReceiveCmd(uint8_t cmd);


#endif 


