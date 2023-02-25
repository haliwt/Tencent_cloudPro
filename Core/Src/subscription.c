#include "subscription.h"
#include <string.h>
#include <stdlib.h>
#include "cmd_link.h"
#include "run.h"
#include "gpio.h"
#include "esp8266.h"
#include "wifi_fun.h"
#include "usart.h"
#include "buzzer.h"

 
//处理腾讯云下发的数据
/*******************************************************************************
   **
   *Function Name:void Receive_Data_FromCloud_Data(int type, char *str)
   *Function: receive data from tencent cloud-subscription of topic
   *Input Ref: module , str ->data
   *Return Ref:NO
   *
********************************************************************************/
void Receive_Data_FromCloud_Data(int type, char *str)
{
   uint8_t   iNameLen = 0;
    char  *p_cName = 0, *p_cPos = str;

       esp8266data.rx_data_len=strlen(str);

      if (type == JSOBJECT) {
        /* Get Key */
        p_cPos = strchr(p_cPos, '"');
        if (!p_cPos) {
            return ;
        }
        p_cName = ++p_cPos;
        p_cPos = strchr(p_cPos, '"');
        if (!p_cPos) {
            return ;
        }
        iNameLen = p_cPos - p_cName;
      esp8266data.rx_data_name_len=iNameLen;

        /* Get Value */
        p_cPos = strchr(p_cPos, ':');
      
      
       }

   
   }

 /*******************************************************************************
**
*Function Name:void Publish_Data_ToCloud(void)
*Function: dy
*Input Ref: 
*Return Ref:NO
*
********************************************************************************/
void Subscriber_Data_FromCloud_Handler(void)
{
   uint8_t *device_massage;

         device_massage = (uint8_t *)malloc(128);
          run_t.randomName[0]=HAL_GetUIDw0();
      
         sprintf((char *)device_massage,"AT+TCMQTTSUB=\"$thing/down/property/%s/UYIJIA01-%d\",0\r\n", PRODUCT_ID, run_t.randomName[0]);
         HAL_UART_Transmit(&huart2, device_massage, strlen((const char *)device_massage), 5000); 
         free(device_massage);
  



   
 }
/*******************************************************************************
**
*Function Name:void Subscribe_Rx_IntHandler(void)
*Function: interrupt USART2 receive data fun
*Input Ref: 
*Return Ref:NO
*
********************************************************************************/
void Subscribe_Rx_Interrupt_Handler(void)
{
 
    switch(esp8266data.rx_data_state)
      {
      case 0:  //#0

            
         if(UART2_DATA.UART_DataBuf[0] == '"')  //hex :54 - "T" -fixed
            esp8266data.rx_data_state=1; //=1
          else{
               esp8266data.rx_counter=0;
            
            }
         break;

      case 1:
      
         if(UART2_DATA.UART_DataBuf[0] == 'p')  //hex :54 - "T" -fixed
            esp8266data.rx_data_state=2; //=1
          else{
               esp8266data.rx_counter=0;
            
            }
            
         break;
      case 2: //#1
             if(UART2_DATA.UART_DataBuf[0] == 'a')  //hex :4B - "K" -fixed
            esp8266data.rx_data_state=3; //=1
         else{
            esp8266data.rx_data_state =0;
             esp8266data.rx_counter=0;
         }
         break;
            
        case 3:
            if(UART2_DATA.UART_DataBuf[0] == 'r')  //hex :4B - "K" -fixed
            esp8266data.rx_data_state=4; //=1
         else{
           esp8266data.rx_data_state =0;
             esp8266data.rx_counter=0;
         }
        
        break;
        
        case 4:
            if(UART2_DATA.UART_DataBuf[0] == 'a')  //hex :4B - "K" -fixed
            esp8266data.rx_data_state=5; //=1
         else{
            esp8266data.rx_data_state =0;
             esp8266data.rx_counter=0;
         }
        
        break;

      case 5:
       if(UART2_DATA.UART_DataBuf[0] == 'm')  //hex :4B - "K" -fixed
         esp8266data.rx_data_state=6; //=1
         else{
           esp8266data.rx_data_state=0;
            esp8266data.rx_counter=0;
         }
            
      break;

      
      case 6:
       if(UART2_DATA.UART_DataBuf[0] == 's')  //hex :4B - "K" -fixed
         esp8266data.rx_data_state=7; //=1
         else{
           esp8266data.rx_data_state =0;
            esp8266data.rx_counter=0;
         }
            
      break;

      case 7:
       if(UART2_DATA.UART_DataBuf[0] == '"')  //hex :4B - "K" -fixed
         esp8266data.rx_data_state=8; //=1
         else{
           esp8266data.rx_data_state =0;
            esp8266data.rx_counter=0;
         }
            
      break;

        case 8:
       if(UART2_DATA.UART_DataBuf[0] == ':')  //hex :4B - "K" -fixed
         esp8266data.rx_data_state=9; //=1
         else{
           esp8266data.rx_data_state =0;
            esp8266data.rx_counter=0;
         }
            
      break;


      case 9:
       if(UART2_DATA.UART_DataBuf[0] == '{')  //hex :4B - "K" -fixed
         esp8266data.rx_data_state=10; //=1
         else{
           esp8266data.rx_data_state =0;
            esp8266data.rx_counter=0;
         }
            
      break;
         
     case 10:
        
         if(esp8266data.rx_data_success==0){
         UART2_DATA.UART_Data[esp8266data.rx_counter] = UART2_DATA.UART_DataBuf[0];
            esp8266data.rx_counter++ ;
            
         if(UART2_DATA.UART_DataBuf[0]==0x7D) //'}') // end
         {
            esp8266data.rx_data_success=1;
            esp8266data.rx_data_state=0;
            esp8266data.rx_counter=0;
            
          
         }
		 else if(UART2_DATA.UART_Data[8]==0x7D){
		             esp8266data.rx_data_success=1;
					 esp8266data.rx_data_state=0;
					 esp8266data.rx_counter=0;


		 }
		 else if(esp8266data.rx_counter >9){ 
		 	if(UART2_DATA.UART_Data[esp8266data.rx_counter ] != 0x72){

			   esp8266data.rx_data_state=10; 

		 	}
			else{
				esp8266data.rx_data_success=1;
	            esp8266data.rx_data_state=0;
	            esp8266data.rx_counter=0;
			}

		  }
         else{

            esp8266data.rx_data_state=10; 
			

         }
        }
            
      break;



        default:
         
         
       break;
      }
 

}
/*******************************************************************************
**
*Function Name:void Subscribe_Rx_IntHandler(void)
*Function: interrupt USART2 receive data fun
*Input Ref: +TCMQTTCONN:OK
*Return Ref:NO
*
********************************************************************************/
void Wifi_Rx_InputInfo_Handler(void)
{
    
    
          strcpy((char *)esp8266data.data, (const char *)UART2_DATA.UART_Data);
          esp8266data.data_size = UART2_DATA.UART_Cnt;


		   if(wifi_t.soft_ap_config_flag==1){

               if(strstr((const char*)esp8266data.data,"+TCSAP:WIFI_CONNECT_SUCCESS")){
              		esp8266data.soft_ap_config_success=1;
					wifi_t.soft_ap_config_flag=0;
               	}

			}
            else{
				  if(strstr((const char*)esp8266data.data,"+TCMQTTCONN:OK")){
	              esp8266data.esp8266_login_cloud_success=1;
	              esp8266data.rx_link_cloud_flag=0;
			  }
           
           }
         UART2_DATA.UART_Flag = 0;
         UART2_DATA.UART_Cnt=0;
         
        
            
}
/*******************************************************************************
    **
    *Function Name:void Tencent_Cloud_Rx_Handler(void)
    *Function: 
    *Input Ref: +TCMQTTCONN:OK
    *Return Ref:NO
    *
********************************************************************************/
void Tencent_Cloud_Rx_Handler(void)
{
    uint8_t i;
    static uint8_t wind_hundred, wind_decade,wind_unit,temp_decade,temp_unit;
    if( esp8266data.rx_data_success==1){
         esp8266data.rx_data_success=0;
    
    if(strstr((char *)UART2_DATA.UART_Data,"nowtemperature\":")){ //WT.EDIT 2023.update
              return ;
     }

    if(strstr((char *)UART2_DATA.UART_Data,"Humidity\":")){
              return ;
     }

   if(strstr((char *)UART2_DATA.UART_Data,"open\":0")){
			   run_t.response_wifi_signal_label = OPEN_OFF_ITEM;
		 
	}
	if(strstr((char *)UART2_DATA.UART_Data,"open\":1")){
	   
	   run_t.response_wifi_signal_label = OPEN_ON_ITEM;
	}
	if(strstr((char *)UART2_DATA.UART_Data,"ptc\":0")){
            if(run_t.gPower_flag ==POWER_ON){
	           run_t.response_wifi_signal_label = PTC_OFF_ITEM;
	         
             }
			
    }
    if(strstr((char *)UART2_DATA.UART_Data,"ptc\":1")){
            if(run_t.gPower_flag ==POWER_ON){
	           // run_t.gDry=1;
			  run_t.response_wifi_signal_label = PTC_ON_ITEM;
				
            }

    }
    if(strstr((char *)UART2_DATA.UART_Data,"Anion\":0")){
          if(run_t.gPower_flag ==POWER_ON){
	          //  run_t.gPlasma=0;
			run_t.response_wifi_signal_label = ANION_OFF_ITEM;
		    
             }
		 
    }
    if(strstr((char *)UART2_DATA.UART_Data,"Anion\":1")){
            if(run_t.gPower_flag ==POWER_ON){
            //run_t.gPlasma=1;
			run_t.response_wifi_signal_label = ANION_ON_ITEM;
		
            }
    }
    if(strstr((char *)UART2_DATA.UART_Data,"sonic\":0")){
            if(run_t.gPower_flag ==POWER_ON){
           // run_t.gUlransonic=0;
			run_t.response_wifi_signal_label = SONIC_OFF_ITEM;
        
                
            }
		
    }
    if(strstr((char *)UART2_DATA.UART_Data,"sonic\":1")){
            if(run_t.gPower_flag ==POWER_ON){
            run_t.gUlransonic=1;
			run_t.response_wifi_signal_label = SONIC_ON_ITEM;
       
           }
			
    }
    if(strstr((char *)UART2_DATA.UART_Data,"state\":1")){
           if(run_t.gPower_flag ==POWER_ON){
            //run_t.gModel=1;
			run_t.response_wifi_signal_label = STATE_ON_ITEM;
        	}
		  
    }
    if(strstr((char *)UART2_DATA.UART_Data,"state\":2")){
            if(run_t.gPower_flag ==POWER_ON){
           // run_t.gModel=2;
			run_t.response_wifi_signal_label = STATE_OFF_ITEM;
            }
			
    }
    if(strstr((char *)UART2_DATA.UART_Data,"temperature")){

	        if(run_t.gPower_flag ==POWER_ON){
			run_t.response_wifi_signal_label = TEMPERATURE_ITEM;
            
	        }
			
    }
   if(strstr((char *)UART2_DATA.UART_Data,"find")){

			if(run_t.gPower_flag ==POWER_ON){

			run_t.response_wifi_signal_label= FAN_ITEM;
			
            
		}
		
    }
   
   }
   switch(run_t.response_wifi_signal_label){

      case OPEN_OFF_ITEM:
	  
        MqttData_Publish_SetOpen(0); //WT.EDIT add
        run_t.wifi_gPower_On= 0;
	    run_t.gPower_On = POWER_OFF;
        run_t.gPower_flag =POWER_OFF;
		run_t.RunCommand_Label=POWER_OFF;

		SendWifiCmd_To_Order(WIFI_POWER_OFF);

       	 Buzzer_KeySound();
   
        run_t.response_wifi_signal_label=0xff;
	  break;

	  case OPEN_ON_ITEM:
	  	
        MqttData_Publish_SetOpen(1);  //WT.EDIT new add item
	   run_t.wifi_gPower_On= POWER_ON;
       run_t.gPower_On = POWER_ON;
	   run_t.gPower_flag =POWER_ON;
	   run_t.RunCommand_Label=POWER_ON;
	   SendWifiCmd_To_Order(WIFI_POWER_ON);
	     Buzzer_KeySound();
	   run_t.response_wifi_signal_label=0xff;

	  break;

	  case PTC_ON_ITEM:
	  if(run_t.gPower_flag ==POWER_ON){
         MqttData_Publish_SetPtc(0x01);
	     run_t.gDry=1;
		 SendWifiCmd_To_Order(WIFI_PTC_ON);
         Buzzer_KeySound();
	  	}
	   run_t.response_wifi_signal_label=0xff;
	   break;

	  case PTC_OFF_ITEM:
	  	if(run_t.gPower_flag ==POWER_ON){
         MqttData_Publish_SetPtc(0);
	     run_t.gDry=0;
		 SendWifiCmd_To_Order(WIFI_PTC_OFF);
         Buzzer_KeySound();
	  	}
	     run_t.response_wifi_signal_label=0xff;
	  	break;

	  case ANION_OFF_ITEM: //"杀菌"
	  	if(run_t.gPower_flag ==POWER_ON){
            MqttData_Publish_SetPlasma(0);
            run_t.gPlasma=0;
			SendWifiCmd_To_Order(WIFI_KILL_OFF);
	  	    Buzzer_KeySound();
	  	}
	   run_t.response_wifi_signal_label=0xff;
	  	break;
		
	  case ANION_ON_ITEM:
	  	if(run_t.gPower_flag ==POWER_ON){
            MqttData_Publish_SetPlasma(1);
            run_t.gPlasma=1;
			SendWifiCmd_To_Order(WIFI_KILL_ON);
	  	    Buzzer_KeySound();
	  	}
	   run_t.response_wifi_signal_label=0xff;
	    break;

	  case SONIC_OFF_ITEM:
        if(run_t.gPower_flag ==POWER_ON){
            MqttData_Publish_SetUltrasonic(0);
            run_t.gUlransonic=0;
			SendWifiCmd_To_Order(WIFI_SONIC_OFF);
			Buzzer_KeySound();
        }
	   run_t.response_wifi_signal_label=0xff;
	  	break;

	  case SONIC_ON_ITEM:
	    if(run_t.gPower_flag ==POWER_ON){
             MqttData_Publish_SetUltrasonic(1);
            run_t.gUlransonic=1;
			SendWifiCmd_To_Order(WIFI_SONIC_ON);
			Buzzer_KeySound();
        }
	   run_t.response_wifi_signal_label=0xff;
	  	break;

	  case STATE_OFF_ITEM:
	  if(run_t.gPower_flag ==POWER_ON){
            MqttData_Publish_SetState(2);
            run_t.gModel=2;
			SendWifiCmd_To_Order(WIFI_MODE_2);
		    Buzzer_KeySound();
        }
	   run_t.response_wifi_signal_label=0xff;
	  break;
		
	  case STATE_ON_ITEM:
	  	 if(run_t.gPower_flag ==POWER_ON){
            MqttData_Publish_SetState(1);
            run_t.gModel=1;
			SendWifiCmd_To_Order(WIFI_MODE_1);
		    Buzzer_KeySound();
        }
	    run_t.response_wifi_signal_label=0xff;
	  	break;

	  case TEMPERATURE_ITEM:
	   if(run_t.gPower_flag ==POWER_ON){

            temp_decade=UART2_DATA.UART_Data[14]-0x30;
            temp_unit=UART2_DATA.UART_Data[15]-0x30;
            run_t.set_temperature_value = temp_decade*10 +  temp_unit;
            if( run_t.set_temperature_value > 40)  run_t.set_temperature_value=40;
            if( run_t.set_temperature_value <20 )   run_t.set_temperature_value=20;
            MqttData_Publis_SetTemp(run_t.set_temperature_value);
			SendWifiData_To_PanelTemp(run_t.set_temperature_value);
            Buzzer_KeySound();
         }
	   run_t.response_wifi_signal_label=0xff;
	  break;

	  case FAN_ITEM:
	    if(run_t.gPower_flag ==POWER_ON){
            if(UART2_DATA.UART_Data[7]==0x31 && UART2_DATA.UART_Data[8]==0x30 && UART2_DATA.UART_Data[9]==0x30){
		           run_t.set_wind_speed_value =100;
            }
			else{
           		 wind_decade=UART2_DATA.UART_Data[7]-0x30;
	       		 wind_unit=UART2_DATA.UART_Data[8]-0x30;
                 run_t.set_wind_speed_value = wind_decade*10 + wind_unit;
			}
            if(run_t.set_wind_speed_value > 100) run_t.set_wind_speed_value=100;
            if(run_t.set_wind_speed_value < 1) run_t.set_wind_speed_value=0;
			  MqttData_Publis_SetFan(run_t.set_wind_speed_value);
    		SendWifiData_To_PanelWindSpeed(run_t.set_wind_speed_value);
          
			Buzzer_KeySound();
            
		}
	  
	    run_t.response_wifi_signal_label=0xff;
	  	break;

	  default :
	  break;


   }

   if(run_t.response_wifi_signal_label==0xff){
		run_t.response_wifi_signal_label=0xf0;

		for(i=0;i<17;i++){
		UART2_DATA.UART_Data[i]=0;


		}

   }
	
   
}
void Wifi_Rx_Beijing_Time_Handler(void)
{

    static uint8_t char_len;
    switch(esp8266data.rx_data_state)
      {
      case 0:  //#0

            
         if(UART2_DATA.UART_DataBuf[0] == 0x01 || UART2_DATA.UART_DataBuf[0] == 0x02 ||UART2_DATA.UART_DataBuf[0] == 0x03 ||UART2_DATA.UART_DataBuf[0] == 0x04 
		 	|| UART2_DATA.UART_DataBuf[0] == 0x05 ||UART2_DATA.UART_DataBuf[0] == 0x06 ||UART2_DATA.UART_DataBuf[0] == 0x07 ||UART2_DATA.UART_DataBuf[0] == 0x08 
		 	|| UART2_DATA.UART_DataBuf[0] == 0x09 ||UART2_DATA.UART_DataBuf[0] == 0x0a ||UART2_DATA.UART_DataBuf[0] == 0x0b ||UART2_DATA.UART_DataBuf[0] == 0x0c ){ 
             esp8266data.rx_data_state=1; //=1
		  }
		  else{
               esp8266data.rx_counter=0;
               UART2_DATA.UART_Flag = 0;
               UART2_DATA.UART_Cnt=0;              
            }
         break;

	  case 1:
	  	 if(UART2_DATA.UART_DataBuf[0] == ' ')
		  	 esp8266data.rx_data_state=2; //=1
		  else{
		      esp8266data.rx_data_state=0; //=1

          }
	  	break;

      case 2:
      
           wifi_t.real_hours=UART2_DATA.UART_DataBuf[0];
           esp8266data.rx_data_state=3; //=1
         break;

	  case 3:
	  	  if(UART2_DATA.UART_DataBuf[0] == ':')
		  	 esp8266data.rx_data_state=4; //=1
		  else{
		      esp8266data.rx_data_state=0; //=1

          }
	  	break;
      case 4: //#1
      
            wifi_t.real_minutes = UART2_DATA.UART_DataBuf[0];
            esp8266data.rx_data_state=5; //=1
       
         break;

	  case 5:
	  	 if(UART2_DATA.UART_DataBuf[0] == ':')
		  	 esp8266data.rx_data_state=6; //=1
		  else{
		      esp8266data.rx_data_state=0; //=1

          }
	  break;
            
        case 6:
           wifi_t.real_seconds = UART2_DATA.UART_DataBuf[0];
           esp8266data.rx_data_state=7; //=1
           
        
        break;

		case 7:
			if(UART2_DATA.UART_DataBuf[0] == ' ')
		  	 esp8266data.rx_data_state=8; //=1
		  else{
		      esp8266data.rx_data_state=0; //=1

          }

		break;
		  
		case 8:
			 if(UART2_DATA.UART_DataBuf[0] ==20){

		          wifi_t.rx_beijing_decode_flag =1;
				  wifi_t.get_rx_beijing_time_flag=0 ;
				  esp8266data.rx_data_state=0; //=1
				  
		     }
			
			
		break;
        
        

        default:
         
         
       break;
      }




}

void Wifi_Get_Beijing_Time_Handler(void)
{
 /*AT+CIPSNTPTIME?+CIPSNTPTIME:Wed Jan 11 19:31:04 2023 OK */



}

