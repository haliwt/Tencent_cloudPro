#include "esp8266.h"
#include "usart.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "run.h"
#define esp8266_Debug   0
ESP8266DATATypedef esp8266data;


static void InitWifiModule(void);


static uint8_t wifi_inputBuf[20];

//get usart of data
uint8_t *Esp8266GetData(void)
{
  if (UART2_DATA.UART_Flag == 1)
  {
    strcpy((char *)esp8266data.data, (const char *)UART2_DATA.UART_Data);
    esp8266data.data_size = UART2_DATA.UART_Cnt;
	
    //clear has been received data array buffer older data.
    for (; UART2_DATA.UART_Cnt > 0; UART2_DATA.UART_Cnt--)
      UART2_DATA.UART_Data[UART2_DATA.UART_Cnt] = 0;
   
    if(strstr((const char*)esp8266data.data,"ERR CODE")){
       // HAL_Delay(1);//osDelay(1);
       esp8266data.flag ==0xff; 
     
    }else  {
      printf("%s", esp8266data.data);
      esp8266data.flag =1; 
    }  

    if(strstr((const char*)esp8266data.data,"+TCMQTTCONN:OK")){
      esp8266data.flag=2; //esp8266data.flag =1 ->connect to tengxun cloud .
    }
    if(strstr((const char*)esp8266data.data,"+TCMQTTSTATE:0")){ //query MQTT order 
     esp8266data.flag=11; // 0->MQTT已经断开,1->MQTT已经连接
    }
    
    if(esp8266data.flag>14) esp8266data.flag=15;

#if esp8266_Debug
    printf("flag=%d\r\n", esp8266data.flag);
#endif

    UART2_DATA.UART_Flag = 0;
    return esp8266data.data;
  }
  return esp8266data.data;
}
/***************************************************************************
	*
	*Function Name: void Esp8266LinkAp(uint8_t *ssid, uint8_t *passwd)
	*Function: tengxun clound to connect
	*Input Ref:ssid ->setu link name,passwd->password over six numbers 
	*Return Ref: NO
	*
***************************************************************************/
void Esp8266LinkAp(uint8_t *ssid, uint8_t *passwd)
{
  uint8_t *linkap;

  linkap = (uint8_t *)malloc(128);
  HAL_UART_Transmit(&huart2, " \r\n", 3, 1000);
  do
  {
    switch (esp8266data.wifi_RunState)
    {
    case 0: //reset ESP8266
      HAL_UART_Transmit(&huart2, "AT+RST\r\n", strlen("AT+RST\r\n"), 5000);
	  
      esp8266data.wifi_RunState++;
      break;
    case 1: //setup link modle
       HAL_UART_Transmit(&huart2, "AT+CIPMUX=0\r\n", strlen("AT+CIPMUX=0\r\n"), 5000);
	   esp8266data.wifi_RunState++;
  
	break;
    case 2:
      HAL_UART_Transmit(&huart2, "AT+CWMODE=3\r\n", strlen("AT+CWMODE=3\r\n"), 5000);
	  esp8266data.wifi_RunState++;
  
     break;
    //start link to  WiFi
    case 3:
	  sprintf((char *)linkap, "AT+CWSAP=\"%s\",\"%s\"\r\n", ssid, passwd);
      HAL_UART_Transmit(&huart2, (uint8_t *)linkap, strlen((const char *)linkap), 5000);


	break;
//    case 5:
//      sprintf((char *)linkap, "AT+CWJAP=\"%s\",\"%s\"\r\n", ssid, passwd);
//      HAL_UART_Transmit(&huart2, (uint8_t *)linkap, strlen((const char *)linkap), 5000);
//     
//      break;

    default:
      break;
    }
  } while (  esp8266data.wifi_RunState < 3);

  free(linkap);
}
/***********************************************************
	*
	*Function Name: void Esp8266LinkloTExplorer(void)
	*Function: tengxun cloud to connect
	*Input Ref:NO
	*Return Ref: NO
	*
************************************************************/
void Esp8266LinkloTExplorer(void)
{
  uint8_t *device_massage;

  device_massage = (uint8_t *)malloc(128);
  do
  {
    switch (esp8266data.flag)
    {
     //setup register information
    case 6:
    case 7:
    case 8:
      sprintf((char *)device_massage, "AT+TCDEVINFOSET=1,\"%s\",\"%s\",\"%s\"\r\n", PRODUCT_ID, DEVICE_SECRET,DEVUICE_NAME);
      HAL_UART_Transmit(&huart2, device_massage, strlen((const char *)device_massage), 5000);
    
      break;

    case 9:
    case 10:
      HAL_UART_Transmit(&huart2, "AT+TCDEVREG\r\n", strlen("AT+TCMQTTDISCONN\r\n"), 5000); //dynamic registration
     
      break;

    case 11:
      HAL_UART_Transmit(&huart2, "AT+TCMQTTCONN=1,5000,240,1,1\r\n", strlen("AT+TCMQTTCONN=1,5000,240,1,1\r\n"), 5000);//start link 
  
      break;

    case 12:
      HAL_UART_Transmit(&huart2, "AT+TCMQTTSTATE?\r\n", strlen("AT+TCMQTTSTATE?\r\n"), 5000);
   
      break;

    case 13:
         sprintf((char *)device_massage, "AT+TCMQTTUNSUB=\"$thing/down/property/%s/%s\",0\r\n", PRODUCT_ID, DEVUICE_NAME);
         HAL_UART_Transmit(&huart2, device_massage, strlen((const char *)device_massage), 5000);
        
    break;
    case 14:
      sprintf((char *)device_massage, "AT+TCMQTTSUB=\"$thing/down/property/%s/%s\",0\r\n", PRODUCT_ID, DEVUICE_NAME);
      HAL_UART_Transmit(&huart2, device_massage, strlen((const char *)device_massage), 5000);
    
      break;

    default:
      esp8266data.flag = 15;
      break;
    }
  } while (esp8266data.flag < 15);

  free(device_massage);
}


/****************************************************************************************************
**
*Function Name:static void initBtleModule(void)
*Function: 
*Input Ref: 
*Return Ref:NO
*
****************************************************************************************************/
static void InitWifiModule(void)
{
	

	while(run_t.wifi_init_flag==0)
	{
		run_t.wifi_init_flag++;
        HAL_UART_Abort(&huart2);
		HAL_Delay(50);
		WIFI_IC_ENABLE();
	    HAL_UART_Transmit(&huart2, "AT+RST\r\n", strlen("AT+RST\r\n"), 5000);
		HAL_Delay(500);
		
		
	}
	HAL_UART_Abort(&huart2);
}

/****************************************************************************************************
**
*Function Name:void Wifi_Link_SmartPhone_Fun(void)
*Function: 
*Input Ref: 
*Return Ref:NO
*
****************************************************************************************************/
void Wifi_Link_SmartPhone_Fun(void)
{
   static uint8_t wifi_cw=0xff,wifi_cwsap=0xff;

   InitWifiModule();
    

   if(run_t.wifi_init_flag ==1){
     
      run_t.gTimer_wifi_1s++;
	  if(run_t.gTimer_wifi_1s>5){
	  	  run_t.gTimer_wifi_1s=0;
		 if(wifi_cw != run_t.wifi_cwmode_flag){
		    wifi_cw = run_t.wifi_cwmode_flag;
			
         	HAL_UART_Transmit(&huart2, "AT+CWMODE=3\r\n", strlen("AT+CWMODE=3\r\n"), 5000);
        	HAL_Delay(500);
			run_t.wifi_cwsap_flag =1;

		 }
	  }

	  if(run_t.wifi_cwsap_flag ==1){
	  	  run_t.wifi_cwsap_flag =0;

         HAL_UART_Transmit(&huart2, "AT+CWSAP=\"YUYIJIA_S03\",\"12345678\",4,4\r\n", strlen("AT+CWSAP=\"YUYIJIA_S03\",\"12345678\",4,4\r\n"), 5000);
         HAL_Delay(10000);
         esp8266data.esp8266_smartphone_flag =1;
		 esp8266data.esp8266_timer_1s=0;
	  }


   }


}

/****************************************************************************************************
**
*Function Name:void Wifi_Link_SmartPhone_Fun(void)
*Function: dy
*Input Ref: 
*Return Ref:NO
*
****************************************************************************************************/
void SmartPhone_LinkTengxunCloud(void)
{
   
    uint8_t *device_massage;

    device_massage = (uint8_t *)malloc(128);

	if(esp8266data.esp8266_smartphone_flag ==1){
		
        if(esp8266data.esp8266_timer_1s >5){
		   esp8266data.esp8266_timer_1s=0;
		   esp8266data.esp8266_smartphone_flag=0; //return this function

	      sprintf((char *)device_massage, "AT+TCPRDINFOSET=1,\"%s\",\"%s\",\"%s\"\r\n", PRODUCT_ID, DEVICE_SECRET,DEVUICE_NAME);
	      HAL_UART_Transmit(&huart2, device_massage, strlen((const char *)device_massage), 5000);
          HAL_Delay(2000);

		  esp8266data.esp8266_dynamic_reg_flag=1;
		 
        }
		

	}

	if(esp8266data.esp8266_dynamic_reg_flag==1){

         esp8266data.esp8266_dynamic_reg_flag=0;
		 HAL_UART_Transmit(&huart2, "AT+TCDEVREG\r\n", strlen("AT+TCDEVREG\r\n"), 5000); //动态注册 
	     HAL_Delay(2000);
		 esp8266data.esp8266_link_cloud_flag =1;
         esp8266data.esp8266_timer_link_1s=0;

     }

	if(esp8266data.esp8266_link_cloud_flag==1){
		
       if(esp8266data.esp8266_timer_link_1s > 5){
	   	esp8266data.esp8266_timer_link_1s=0;
	     esp8266data.esp8266_link_cloud_flag=0;

       HAL_UART_Transmit(&huart2, "AT+TCMQTTCONN=1,5000,240,0,1\r\n", strlen("AT+TCMQTTCONN=1,5000,240,0,1\r\n"), 5000);//开始连接
       HAL_Delay(2000);

	  }
	}
 	 free(device_massage);

}


