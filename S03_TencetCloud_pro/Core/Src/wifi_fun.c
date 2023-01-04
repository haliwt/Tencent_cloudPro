#include "wifi_fun.h"
#include "cmd_link.h"
#include "run.h"
#include "fan.h"
#include "tim.h"
#include "special_power.h"
#include "esp8266.h"

WIFI_FUN   wifi_t;

void (*PowerOn)(void);
void (*PowerOff)(void);
void (*Ai_Fun)(uint8_t sig);


void (*SetTimes)(void);
void (*SetTemperature)(void);

static void Wifi_RunCmd(uint8_t sig);
static void wifiPowerOn_After_data_update(void);





uint8_t temp[7];


void PowerOn_Host(void (* poweronHandler)(void))
{
    PowerOn = poweronHandler;  

}


void PowerOff_Host(void(*poweroffHandler)(void))
{
   PowerOff = poweroffHandler;

}
/****************************************************************
     * 
     * Function Name:void AI_Function_Host(void(*AIhandler)(uint8_t sig))
     * Function: take with reference of function pointer
     * 
     * 
****************************************************************/
void AI_Function_Host(void(*AIhandler)(uint8_t sig))
{
         Ai_Fun=AIhandler;
}

void SetTimeHost(void(*timesHandler)(void))
{
     SetTimes = timesHandler;

}


void SetTemperatureHost(void(*temperatureHandler)(void))
{
      SetTemperature = temperatureHandler ;

}


/***********************************************
	*
	*Function Name: void Wifi_Mode(void)
	*Funciton : wifi power on machine
	*           default is AI mode
	*
***********************************************/
void Wifi_Mode(void)
{
   
if(esp8266data.esp8266_login_cloud_success==1){ //当WIFI连接成功，

   if( run_t.gPower_flag ==1){ //from smartPhone receive signal ->smartPhone operation 

      wifi_t.wifi_power = 0xf0;
      PowerOn(); //default AI 
      SendWifiCmd_To_Order(0x80); //send displayPanel operation Power On action.
      
  
      wifi_t.WifiMode =1;
      wifiPowerOn_After_data_update();
     
           
   }

   if(run_t.gPower_flag ==0 ){ //from smartPhone operation and receive data form tencent cloud .
          wifi_t.wifi_power = 0xff;
			 PowerOff();
      
          run_t.gFan_continueRun =1;
          run_t.gFan_counter=0;
          wifi_t.WifiMode =0;
          SendWifiCmd_To_Order(0x81);
		  
           
    }
   
     
      
    
  }
  }

/***********************************************
   *
   *Function Name: void Wifi_RunCmd(void)
   *Funciton : wifi power on default is AI mode
   *
   *
***********************************************/
static void Wifi_RunCmd(uint8_t sig)
{
   Ai_Fun(sig); //调用函数地址,有参数的函数

}




/***********************************************
   *
   *Function Name: void Wifi_RunCmd(void)
   *Funciton : wifi power on default is AI mode
   *
   *
***********************************************/
static void wifiPowerOn_After_data_update(void)
{



}
/***********************************************
   *
   *Function Name: void Wifi_RunCmd(void)
   *Funciton : separately update value 
   *
   *
***********************************************/
void wifiDisplayTemperature_Humidity(void)
{
 // mcu_dp_value_update(DPID_DISPTEMP,wifi_t.dispTemperatureValue); //VALUE型数据上报;
 /// mcu_dp_value_update(DPID_DISPHUM,wifi_t.dispHumidityValue); //VALUE型数据上报;

}

void wifiUpdate_Power_Status(uint8_t pvalue)
{

  // mcu_dp_bool_update(DPID_START,pvalue); //BOOL型数据上报;
}
void wifiUpdate_Kill_Status(uint8_t kvalue)
{
  // mcu_dp_bool_update(DPID_KILL,kvalue); //BOOL型数据上报;
}
void wifiUpdate_AI_Status(uint8_t aiv)
{
 // mcu_dp_enum_update(DPID_MODE,aiv); //枚举型数据上报;
}
void wifiUpdate_Dry_Status(uint8_t dvalue)
{
   // mcu_dp_bool_update(DPID_HEAT,dvalue); //BOOL型数据上报;
}


void wifiUpdate_SetTimeValue(uint8_t tv)
{
  // mcu_dp_value_update(DPID_SETTIME,tv); //VALUE型数据上报;

}

void wifiUpdate_SetTemperatureValue(uint8_t temp)
{
  // mcu_dp_value_update(DPID_SETTEMP,temp); //VALUE型数据上报;
}


