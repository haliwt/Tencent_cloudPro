#include "subscription.h"
#include <string.h>
#include <stdlib.h>
#include "cmd_link.h"
#include "run.h"
#include "gpio.h"
#include "esp8266.h"
#include "wifi_fun.h"
#include "usart.h"

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
void Subscriber_Data_FromCloud_Handler()
{
   uint8_t *device_massage;

         device_massage = (uint8_t *)malloc(128);
      
         sprintf((char *)device_massage,"AT+TCMQTTSUB=\"$thing/down/property/%s/%s\",0\r\n", PRODUCT_ID, DEVUICE_NAME);
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
    static uint8_t char_len;
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
            char_len = esp8266data.rx_counter;
            esp8266data.rx_data_success=1;
            esp8266data.rx_data_state=0;
            esp8266data.rx_counter=0;
            UART2_DATA.UART_Data[char_len];
          
               
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

    static uint8_t wind_hundred, wind_decade,wind_unit,temp_decade,temp_unit;
    if( esp8266data.rx_data_success==1){
        esp8266data.rx_data_success=0;
   if(strstr((char *)UART2_DATA.UART_Data,"open\":0")){
			   
			run_t.wifi_gPower_On= 0;
            run_t.gPower_On = POWER_OFF;
   			SendWifiCmd_To_Order(WIFI_POWER_OFF);
	}
	else if(strstr((char *)UART2_DATA.UART_Data,"open\":1")){
				   
				run_t.wifi_gPower_On= 1;
	            run_t.gPower_On = POWER_ON;
				SendWifiCmd_To_Order(WIFI_POWER_ON);
	}
	else if(strstr((char *)UART2_DATA.UART_Data,"ptc\":0")){
           
            run_t.gDry=0;
            SendWifiCmd_To_Order(WIFI_PTC_OFF);
    }
    else if(strstr((char *)UART2_DATA.UART_Data,"ptc\":1")){
          
            run_t.gDry=1;
			SendWifiCmd_To_Order(WIFI_PTC_ON);

    }
    else if(strstr((char *)UART2_DATA.UART_Data,"Anion\":0")){
          
            run_t.gPlasma=0;
	       SendWifiCmd_To_Order(WIFI_KILL_OFF);

    }
    else if(strstr((char *)UART2_DATA.UART_Data,"Anion\":1")){
           
            run_t.gPlasma=1;
			SendWifiCmd_To_Order(WIFI_KILL_ON);

    }
    else if(strstr((char *)UART2_DATA.UART_Data,"sonic\":0")){
            
            run_t.gUlransonic=0;
            SendWifiCmd_To_Order(WIFI_SONIC_OFF);
    }
    else if(strstr((char *)UART2_DATA.UART_Data,"sonic\":1")){
            
            run_t.gUlransonic=1;
            SendWifiCmd_To_Order(WIFI_SONIC_ON);
    }
    else if(strstr((char *)UART2_DATA.UART_Data,"state\":1")){
          
            run_t.gModel=1;
            SendWifiCmd_To_Order(WIFI_MODE_1);
    }
    else if(strstr((char *)UART2_DATA.UART_Data,"state\":2")){
           
            run_t.gModel=2;
            SendWifiCmd_To_Order(WIFI_MODE_2);
    }
    else if(strstr((char *)UART2_DATA.UART_Data,"temperature")){
           
            temp_decade=UART2_DATA.UART_Data[14]-0x30;
            temp_unit=UART2_DATA.UART_Data[15]-0x30;
            run_t.set_temperature_value = temp_decade*10 +  temp_unit;
			SendWifiData_To_PanelTemp(run_t.set_temperature_value);
    }
    else if(strstr((char *)UART2_DATA.UART_Data,"find")){
          
            wind_hundred=UART2_DATA.UART_Data[7]-0x30;
            wind_decade=UART2_DATA.UART_Data[8]-0x30;
	        wind_unit=UART2_DATA.UART_Data[9]-0x30;
            run_t.set_wind_speed_value = wind_hundred *100 + wind_decade*10 + wind_unit;
    		SendWifiData_To_PanelWindSpeed(run_t.set_wind_speed_value);
    }
    }

}
