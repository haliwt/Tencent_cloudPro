#include "wifi_fun.h"
#include "cmd_link.h"
#include "run.h"
#include "fan.h"
#include "tim.h"
#include "special_power.h"
#include "esp8266.h"
#include "publish.h"
#include "subscription.h"

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
   *Funciton : separately update value 
   *
   *
***********************************************/
void RunWifi_Command_Handler(void)
{
     static uint8_t first_sub,first_publish,disconnect;
     switch(wifi_t.runCommand_order_lable){


	    case wifi_has_benn_connected:
		  disconnect =0;
	      first_sub=0;
		  first_sub=0;
		  SmartPhone_TryToLink_TencentCloud();
		  if(esp8266data.esp8266_login_cloud_success==1){
		  	    esp8266data.rx_link_cloud_flag=0;
         	//	SendWifiData_To_Cmd(1);//To tell display panel wifi be connetor to tencent cloud is success
				esp8266data.gTimer_publish_timing=0;
	            esp8266data.gTimer_subscription_timing=0;
				wifi_t.runCommand_order_lable = wifi_tencent_init_data;
				
		  }
		  else{

		       wifi_t.runCommand_order_lable = wifi_link_tencent_cloud;

		  }


	    break;

        case wifi_link_tencent_cloud:
			
			Wifi_SoftAP_Config_Handler();
	        SmartPhone_LinkTencent_Cloud();
	        esp8266data.gTimer_publish_timing=0;
	        esp8266data.gTimer_subscription_timing=0;

			if(esp8266data.esp8266_login_cloud_success==1){
				disconnect =0;
			    wifi_t.runCommand_order_lable = wifi_tencent_init_data;
			}

	    break;

	  case wifi_tencent_init_data:
		if(esp8266data.gTimer_subscription_timing>2 && first_sub==0  ){
		 	esp8266data.gTimer_subscription_timing=0;
		    first_sub++;
			 Subscriber_Data_FromCloud_Handler();
             wifi_t.runCommand_order_lable= wifi_rx_tencent_cloud_data;
		 }

		  if(esp8266data.gTimer_publish_timing>4 && first_publish == 0){
				first_publish++;
	           esp8266data.gTimer_publish_timing=0;
	          
				Publish_Data_ToCloud_Handler();
				wifi_t.runCommand_order_lable= wifi_rx_tencent_cloud_data;
	           
	   	    }
		
		
       break;

	   case wifi_rx_tencent_cloud_data: 
			
		    Tencent_Cloud_Rx_Handler();

	   break;

	   case wifi_disconnect:
          if(disconnect == 0){
		  	 disconnect ++;
		    wifi_Disconnect_Fun();
          }
		  esp8266data.esp8266_login_cloud_success=0;
	   break;

	   default:

	   break;
	 
     }
	 	
}

