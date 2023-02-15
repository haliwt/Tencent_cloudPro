#include "wifi_fun.h"
#include "cmd_link.h"
#include "run.h"
#include "fan.h"
#include "tim.h"
#include "special_power.h"
#include "esp8266.h"
#include "publish.h"
#include "subscription.h"
#include "dht11.h"
#include "usart.h"
#include "mqtt_iot.h"


WIFI_FUN   wifi_t;

void (*PowerOn)(void);
void (*PowerOff)(void);
void (*Ai_Fun)(uint8_t sig);


void (*SetTimes)(void);
void (*SetTemperature)(void);


void PowerOn_Host(void (* poweronHandler)(void))
{
    PowerOn = poweronHandler;  

}


void PowerOff_Host(void(*poweroffHandler)(void))
{
   PowerOff = poweroffHandler;

}
uint8_t first_connect;
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
   *Function Name: void Wifi_RunCmd(void)
   *Funciton : separately update value 
   *
   *
***********************************************/
void RunWifi_Command_Handler(void)
{
     static uint8_t first_sub, get_rx_beijing_time_flag;
	 static uint8_t beijing_flag,gamt_recode;

	
	if(run_t.gPower_flag == POWER_ON){
     switch(wifi_t.runCommand_order_lable){

         
	    case wifi_has_been_connected:
		  first_sub=0;
		  first_connect=0;
         
		  wifi_t.get_rx_beijing_time_flag=0;
		 SmartPhone_TryToLink_TencentCloud();
	     if(esp8266data.esp8266_login_cloud_success==1){
		  	esp8266data.rx_link_cloud_flag=0;
			esp8266data.gTimer_publish_timing=0;
	        esp8266data.gTimer_subscription_timing=0;
			wifi_t.has_been_login_flag = 1;
			wifi_t.runCommand_order_lable = wifi_tencent_subscription_data;
			
				
		  }
		  else{
		  		wifi_t.runCommand_order_lable =0xff;

		  }
		  if(wifi_t.restart_link_tencent_cloud ==1)wifi_t.runCommand_order_lable= wifi_link_tencent_cloud;
		break;

        case wifi_link_tencent_cloud:
			
			Wifi_SoftAP_Config_Handler();
	        SmartPhone_LinkTencent_Cloud();
	        esp8266data.gTimer_publish_timing=0;
	        esp8266data.gTimer_subscription_timing=0;

			if(esp8266data.esp8266_login_cloud_success==1){
			    wifi_t.runCommand_order_lable = wifi_tencent_publish_init_data;
			}
           wifi_t.restart_link_tencent_cloud++;
	    break;

	  	
	  	case wifi_tencent_publish_init_data:
		  
	       if(esp8266data.gTimer_publish_timing>2 ){
	           esp8266data.gTimer_publish_timing=0;
	           esp8266data.gTimer_subscription_timing=0;
			   esp8266data.gTimer_publish_dht11 =0;
				Publish_Data_ToCloud_Handler();
				wifi_t.runCommand_order_lable= wifi_tencent_subscription_data;
	           
	   	    }
		   if(wifi_t.restart_link_tencent_cloud ==1)wifi_t.runCommand_order_lable= wifi_link_tencent_cloud;
		
		 
       	break;

		
		case wifi_tencent_subscription_data:
			if(esp8266data.gTimer_subscription_timing>3 && first_sub==0  ){
				first_sub++;
			esp8266data.gTimer_subscription_timing=0;
			 esp8266data.gTimer_publish_timing=0;
			 Subscriber_Data_FromCloud_Handler();
			 wifi_t.runCommand_order_lable= wifi_publish_update_tencent_cloud_data;
					
			}
			
			if(esp8266data.gTimer_subscription_timing>59){
                 esp8266data.gTimer_subscription_timing=0;
	            Subscriber_Data_FromCloud_Handler();
			
				wifi_t.runCommand_order_lable= wifi_publish_update_tencent_cloud_data;

            }

		if(esp8266data.gTimer_publish_dht11 >12)wifi_t.runCommand_order_lable= wifi_tencent_publish_dht11_data;
          if(wifi_t.restart_link_tencent_cloud ==1)wifi_t.runCommand_order_lable= wifi_link_tencent_cloud;
		   break;

	   	case wifi_publish_update_tencent_cloud_data://5
            
            wifi_t.get_rx_beijing_time_flag=0;
            if(esp8266data.gTimer_publish_timing>240 ){
					esp8266data.gTimer_publish_timing=0;
		            Publish_Data_ToCloud_Login_Handler();

		       
           }
           if(esp8266data.gTimer_publish_dht11 >60){
		   	  esp8266data.gTimer_publish_dht11=0;
			 
		   	  wifi_t.runCommand_order_lable= wifi_tencent_publish_dht11_data;
           	}
		  if(wifi_t.restart_link_tencent_cloud ==1)wifi_t.runCommand_order_lable= wifi_link_tencent_cloud;
	   break;



	   case wifi_tencent_publish_dht11_data://6
	   	    wifi_t.get_rx_beijing_time_flag=0;
            if(gamt_recode==0){
                 gamt_recode ++;
                 wifi_t.gTimer_get_beijing_time=0;
			}
			esp8266data.gTimer_subscription_timing=0;
	       //	HAL_UART_Receive_IT(&huart2,UART2_DATA.UART_DataBuf,1);
			
            esp8266data.gTimer_publish_dht11=0;
			Update_Dht11_Totencent_Value();
			if(wifi_t.gTimer_get_beijing_time > 180){
			   wifi_t.gTimer_get_beijing_time=0;
			   gamt_recode=0;
			   wifi_t.get_rx_beijing_time_flag=1;
			  UART2_DATA.UART_Cnt=0;
			  
			   wifi_t.gTimer_beijing_time=0;
	           wifi_t.runCommand_order_lable= wifi_get_beijing_time; 

			    
			}
			else{

                wifi_t.runCommand_order_lable= wifi_publish_update_tencent_cloud_data;
               
			}
			
	     if(wifi_t.restart_link_tencent_cloud ==1)wifi_t.runCommand_order_lable= wifi_link_tencent_cloud;  

	   break;

	   case wifi_get_beijing_time://7
	 
		  esp8266data.rx_link_cloud_flag =0;
	   	  wifi_t.get_rx_beijing_time_flag=1;
	   	  if(beijing_flag ==0 && wifi_t.gTimer_beijing_time>1){
			 beijing_flag++;
			 wifi_t.gTimer_beijing_time=0;
             UART2_DATA.UART_Cnt=0;
             Get_BeiJing_Time_Cmd();
             
		     wifi_t.gTimer_beijing_time=0;
	   	  }
	   	  if(wifi_t.gTimer_beijing_time>2){
		  	wifi_t.gTimer_beijing_time=0;
			beijing_flag =0;
			esp8266data.gTimer_publish_timing=0;
		   	Get_Beijing_Time();
			HAL_Delay(1000);
			HAL_Delay(1000);
		     get_rx_beijing_time_flag=1;
	   	   }
		  
          if(get_rx_beijing_time_flag==1){
		  	get_rx_beijing_time_flag=0;
		  	
		  	wifi_t.real_hours = (UART2_DATA.UART_Data[134]-0x30)*10 + UART2_DATA.UART_Data[135]-0x30;
			wifi_t.real_minutes =(UART2_DATA.UART_Data[137]-0x30)*10 + UART2_DATA.UART_Data[138]-0x30;
		     wifi_t.real_seconds = (UART2_DATA.UART_Data[140]-0x30)*10 + UART2_DATA.UART_Data[141]-0x30;
		    if(wifi_t.real_hours < 25 && wifi_t.real_minutes < 60 && wifi_t.real_seconds < 60){
		       SendData_Real_GMT(wifi_t.real_hours,wifi_t.real_minutes,wifi_t.real_seconds);
		    }
	         wifi_t.runCommand_order_lable=wifi_publish_update_tencent_cloud_data;
		   }
		 if(wifi_t.restart_link_tencent_cloud ==1)wifi_t.runCommand_order_lable= wifi_link_tencent_cloud;
		 
	   break;

	   

	   default:

	   break;
	 
     }
     }//run_t.gPower_flag ==POWER_ON
	if(esp8266data.esp8266_login_cloud_success==1){
	 	 esp8266data.rx_link_cloud_flag=0;
	 	  if(first_connect == 0 ){
		  	first_connect ++ ;
            SendWifiData_To_Cmd(0x01) ;
		  }
		  if(wifi_t.get_rx_beijing_time_flag==0){
	          Tencent_Cloud_Rx_Handler();
		  }
		 if(wifi_t.restart_link_tencent_cloud ==1)wifi_t.runCommand_order_lable= wifi_link_tencent_cloud;

     }
  }
 
	 	





