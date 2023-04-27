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
#include "flash.h"


WIFI_FUN   wifi_t;

void (*PowerOn)(void);
void (*PowerOff)(void);
void (*Ai_Fun)(uint8_t sig);


void (*SetTimes)(void);
void (*SetTemperature)(void);
uint8_t get_rx_beijing_time_flag;


static void AutoReconnect_Wifi_Neware_Function(void);


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
     uint32_t temp;
    static uint8_t first_sub,subscription_flag,beijing_time_flag;
	 static uint8_t det_no_wifi_net, beijing_flag;;
	 static uint8_t  update_publish_times=0,get_bj_times=0;

     switch(wifi_t.runCommand_order_lable){

         
	    case wifi_has_been_connected:
		  first_sub=0;
		  first_connect=0;
          update_publish_times=0;
		  wifi_t.get_rx_beijing_time_flag=0;
		 run_t.wifi_config_net_lable=0;
		 
		 
	     if(esp8266data.esp8266_login_cloud_success==1){
		  	esp8266data.rx_link_cloud_flag=0;
			esp8266data.gTimer_publish_timing=0;
	        esp8266data.gTimer_subscription_timing=0;
			wifi_t.has_been_login_flag = 1;
			wifi_t.runCommand_order_lable = wifi_tencent_subscription_data;
			
				
		  }

		break;

        case wifi_link_tencent_cloud: //02

		   if(run_t.gPower_flag == POWER_ON){
			
			Wifi_SoftAP_Config_Handler();
	        SmartPhone_LinkTencent_Cloud();
	        esp8266data.gTimer_publish_timing=0;
	        esp8266data.gTimer_subscription_timing=0;
            update_publish_times=0;
			if(esp8266data.esp8266_login_cloud_success==1){
				esp8266data.gTimer_publish_timing=0;
				 SendWifiData_To_Cmd(0x01) ;	//WT.EDIT 2023.03.02
			    wifi_t.runCommand_order_lable = wifi_tencent_publish_init_data;
				esp8266data.gTimer_subscription_timing=0;
			}
		    }
	    break;

	  	
	  	case wifi_tencent_publish_init_data://03
		  
	       if(esp8266data.gTimer_publish_timing>4 ){ //10
	           esp8266data.gTimer_publish_timing=0;
	           esp8266data.gTimer_subscription_timing=0;
			   esp8266data.gTimer_publish_dht11 =0;

			     Subscriber_Data_FromCloud_Handler();
				 HAL_Delay(200);
	             MqttData_Publish_SetOpen(0x01);
		         HAL_Delay(200);
		         Publish_Data_ToTencent_Initial_Data();
				 HAL_Delay(200);


			   
				
				Subscriber_Data_FromCloud_Handler();
				HAL_Delay(200);
				wifi_t.runCommand_order_lable= wifi_tencent_subscription_data;
	           
	   	    }
		   
		
		 
       	break;

		
		case wifi_tencent_subscription_data://04
           
			if(esp8266data.gTimer_subscription_timing>3 && first_sub==0  ){
				first_sub++;
				 SendWifiData_To_Cmd(0x01) ;	//WT.EDIT 2023.03.02
				 HAL_Delay(500);
				esp8266data.gTimer_subscription_timing=0;
			 	esp8266data.gTimer_publish_timing=0;
			 	Subscriber_Data_FromCloud_Handler();
				HAL_Delay(1000);
			 	wifi_t.runCommand_order_lable= wifi_publish_update_tencent_cloud_data;
				wifi_t.gTimer_get_beijing_time=0;
					
			}
			
			if(esp8266data.gTimer_subscription_timing>59){
                 esp8266data.gTimer_subscription_timing=0;
	            Subscriber_Data_FromCloud_Handler();
			
				wifi_t.runCommand_order_lable= wifi_publish_update_tencent_cloud_data;

            }

		if(esp8266data.gTimer_publish_dht11 >12)wifi_t.runCommand_order_lable= wifi_tencent_publish_dht11_data;

		break;

	   	case wifi_publish_update_tencent_cloud_data://05

		        if(update_publish_times ==0 ){
					update_publish_times++;

                 MqttData_Publish_SetOpen(0x01);
		         HAL_Delay(200);
		         Publish_Data_ToTencent_Update_Data();
				 HAL_Delay(200);

		        }
		       
           
               if(esp8266data.esp8266_login_cloud_success==1){
				wifi_t.get_rx_beijing_time_flag=0;
	            if(esp8266data.gTimer_publish_timing>123 ){
					 esp8266data.gTimer_publish_timing=0;
			         Publish_Data_ToTencent_Update_Data();

			       
	           }

		 
	         
	            if(esp8266data.gTimer_subscription_timing>132 && subscription_flag < 5){
					 subscription_flag ++;
					 wifi_t.gTimer_publish_times =0;
					 esp8266data.gTimer_subscription_timing=0;
			         Subscriber_Data_FromCloud_Handler();

	            }
	            if(wifi_t.gTimer_publish_times > 723 ){
	            	wifi_t.gTimer_publish_times=0;
                    
                   __HAL_UART_CLEAR_OREFLAG(&huart2);
                      __HAL_UART_CLEAR_NEFLAG(&huart2);
                       __HAL_UART_CLEAR_FEFLAG(&huart2);

                 
                  temp=USART2->ISR;
                  temp = USART2->RDR;

                   UART_Start_Receive_IT(&huart2,(uint8_t *)UART2_DATA.UART_DataBuf,1);

	            }

		    while(beijing_time_flag == 1){
                 beijing_time_flag = 0;
			    
                 
				  if(get_bj_times < 3){
				  	 get_bj_times++;
				     wifi_t.runCommand_order_lable= wifi_get_beijing_time;
	                 wifi_t.gTimer_get_beijing_time=0;

				 }
				
				    if(wifi_t.real_hours < 25 && wifi_t.real_minutes < 61){
				 
				       SendData_Real_GMT(wifi_t.real_hours);
					   HAL_Delay(100);
					   SendData_Real_GMT_Minute(wifi_t.real_minutes);
					    HAL_Delay(100);
					   SendData_Real_GMT_Second(wifi_t.real_seconds);
					   HAL_Delay(100);
				    }
                    
                    esp8266data.rx_link_cloud_flag =1; 
	   	            wifi_t.get_rx_beijing_time_flag=0;//disenable get beijing timing
				  
				 }


		 if(esp8266data.gTimer_publish_dht11 >60){
		   	  esp8266data.gTimer_publish_dht11=0;

		   	  wifi_t.runCommand_order_lable= wifi_tencent_publish_dht11_data;
           	}
     
          }


	      AutoReconnect_Wifi_Neware_Function();


			break;



	   case wifi_tencent_publish_dht11_data://6
	   	   wifi_t.get_rx_beijing_time_flag=0;

			esp8266data.gTimer_subscription_timing=0;
			
            esp8266data.gTimer_publish_dht11=0;
			Update_Dht11_Totencent_Value();
			if(wifi_t.gTimer_get_beijing_time > 370){ //
			   wifi_t.gTimer_get_beijing_time=0;
			   get_bj_times=0;
			   wifi_t.get_rx_beijing_time_flag=1;
			  UART2_DATA.UART_Cnt=0;
			  
			   wifi_t.gTimer_beijing_time=0;
	           wifi_t.runCommand_order_lable= wifi_get_beijing_time; 

			    
			}
			else{

                wifi_t.runCommand_order_lable= wifi_publish_update_tencent_cloud_data;
               
			}
	  

	   break;

	   case wifi_get_beijing_time://7
	 
		 esp8266data.rx_link_cloud_flag =0;
	   	  wifi_t.get_rx_beijing_time_flag=1; //enable beijing times
	   	  if(beijing_flag ==0 ){   //&& wifi_t.gTimer_beijing_time>1){
			 beijing_flag++;
			 wifi_t.gTimer_beijing_time=0;
             UART2_DATA.UART_Cnt=0;
             Get_BeiJing_Time_Cmd();
             
	   	  }
	   	  if(wifi_t.gTimer_beijing_time>2){
            wifi_t.gTimer_beijing_time=0;
		    beijing_flag=0;
			esp8266data.gTimer_publish_timing=0;
	
		   	Get_Beijing_Time();
			HAL_Delay(1000);
			HAL_Delay(1000);
		     get_rx_beijing_time_flag=1;
	   	   }
		  
          if(get_rx_beijing_time_flag==1  ){
		  	get_rx_beijing_time_flag=0;
		     beijing_time_flag = 1;
		  	// wifi_t.gTimer_beijing_time=0;
		      beijing_flag=0;
		  	wifi_t.real_hours = (UART2_DATA.UART_Data[134]-0x30)*10 + UART2_DATA.UART_Data[135]-0x30;
			wifi_t.real_minutes =(UART2_DATA.UART_Data[137]-0x30)*10 + UART2_DATA.UART_Data[138]-0x30;
		     wifi_t.real_seconds = (UART2_DATA.UART_Data[140]-0x30)*10 + UART2_DATA.UART_Data[141]-0x30;
		   
			
	         wifi_t.runCommand_order_lable=wifi_publish_update_tencent_cloud_data;
		   }

	      

		 
		 
	   break;

	   

	   default:

	   break;
	 
     }
  
	if(esp8266data.esp8266_login_cloud_success==1){
	 	 esp8266data.rx_link_cloud_flag=0;
	 	  if(first_connect == 0 ){
		  	first_connect ++ ;
		    
            SendWifiData_To_Cmd(0x01) ;
		  }
		  if(wifi_t.get_rx_beijing_time_flag==0){
	          Tencent_Cloud_Rx_Handler();
		  }
		  if(run_t.flash_write_data_flag == 0){
			run_t.flash_write_data_flag=2;
               Flash_Erase_Data();
                Flash_Write_Data();

		 }
		

     }
	
  }
 
	 	


static void AutoReconnect_Wifi_Neware_Function(void)
{
  
     static uint8_t det_no_wifi_net=0;
			 
				if(wifi_t.wifi_reconnect_read_flag == 1 && det_no_wifi_net==0){
					 
						det_no_wifi_net++;
						run_t.auto_link_cloud_flag=0;
						SendWifiData_To_Cmd(0x0) ;
					  run_t.auto_link_cloud_flag=0;
	
					  esp8266data.esp8266_login_cloud_success=0;
	
				  }
	
				   if(wifi_t.wifi_reconnect_read_flag == 1 && run_t.auto_link_cloud_flag==0 ){
				   
					   AutoRepeate_Link_Tencent_Cloud();
					   wifi_t.runCommand_order_lable= wifi_publish_update_tencent_cloud_data;
					}
					
					if(det_no_wifi_net==1){
						
						if(esp8266data.esp8266_login_cloud_success==1){
								 det_no_wifi_net=0;
								 run_t.reconnect_tencent_cloud_flag=0;
								run_t.auto_link_cloud_flag=0xff;
								esp8266data.rx_link_cloud_flag =0;
							   // wifi_t.runCommand_order_lable=wifi_has_been_connected;
								SendWifiData_To_Cmd(0x01) ;
								HAL_Delay(30);
							  
								
								Subscriber_Data_FromCloud_Handler();
								HAL_Delay(300);
	
								Publish_Data_ToTencent_Update_Data();
								HAL_Delay(300);
						}
							
							
						
					}
			


}



