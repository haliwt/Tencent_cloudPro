#include "esp8266.h"
#include "usart.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "run.h"
#include "mqtt_iot.h"



ESP8266DATATypedef esp8266data;

 uint8_t *sub_buf;

char *CloudInfo="+TCMQTTCONN:OK";
char *usart2_tx;

uint8_t usart2_flag;


void InitWifiModule(void);



static uint8_t wifi_inputBuf[20];

/**
 *pdata: pointer of data for send
 *len:  len of data to be sent
 *return: the len of data send success
 * @brief hal api for at data send
 */
uint8_t at_send_data(uint8_t* pdata, uint16_t len)
{
	if(HAL_OK == HAL_UART_Transmit(&huart2, pdata, len, 10000))
	{
		return len;
	}
	else
	{
		return 0;
	}	
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
	
	if(run_t.wifi_config_net_lable==0)
		{
			run_t.wifi_config_net_lable++;
		  //  HAL_UART_Abort(&huart2);
			
			WIFI_IC_ENABLE();
	
	
			run_t.gTimer_wifi_1s=0;
			at_send_data("AT+RESTORE\r\n", strlen("AT+RESTORE\r\n"));
			HAL_Delay(500);
			at_send_data("AT+RST\r\n", strlen("AT+RST\r\n"));
			run_t.gTimer_wifi_1s=0;
			HAL_Delay(1000);
			HAL_Delay(1000);
			
		}
		//HAL_UART_Abort(&huart2);

}


/****************************************************************************************************
	**
	*Function Name:void Wifi_Link_SmartConfig_Fun(void)
	*Function: 
	*Input Ref: 
	*Return Ref:NO
	*
****************************************************************************************************/
void Wifi_Link_SmartConfig_Handler(void)
{
       uint8_t *device_massage;
  

    device_massage = (uint8_t *)malloc(128);

   InitWifiModule();
    
   switch(run_t.wifi_config_net_lable){

   	case 1:

   			WIFI_IC_ENABLE();
         	HAL_UART_Transmit(&huart2, "AT+CWMODE=3\r\n", strlen("AT+CWMODE=3\r\n"), 5000);
        	HAL_Delay(1000);
			
			run_t.wifi_config_net_lable=2;

	break;

	case 2:
	  
	    WIFI_IC_ENABLE();
	     // sprintf((char *)device_massage, "AT+TCSAP=\"%s\"\r\n", DEVUICE_NAME,);
         //HAL_UART_Transmit(&huart2, device_massage, strlen((const char *)device_massage), 5000);

         HAL_UART_Transmit(&huart2, "AT+CWJAP=\"UYIKIA\",\"20329263\"\r\n", strlen("AT+CWJAP=\"UYIKIA\",\"20329263\"\r\n"), 5000);
         HAL_Delay(10000);
		 HAL_Delay(10000);
         run_t.wifi_config_net_lable=3;
	 break;

	 case 3:

		sprintf((char *)device_massage, "AT+TCPRDINFOSET=1,\"%s\",\"%s\",\"%s\"\r\n", PRODUCT_ID, DEVICE_SECRET,DEVUICE_NAME);
	      HAL_UART_Transmit(&huart2, device_massage, strlen((const char *)device_massage), 5000);
          HAL_Delay(1000);
		  HAL_Delay(1000);
		  
	      run_t.wifi_config_net_lable=4;

		
     break;

     case 4:

         esp8266data.esp8266_dynamic_reg_flag=0;
		 HAL_UART_Transmit(&huart2, "AT+TCDEVREG\r\n", strlen("AT+TCDEVREG\r\n"), 5000); //动态注册 
	     HAL_Delay(1000);
		  HAL_Delay(1000);
	      HAL_Delay(1000);
		run_t.wifi_config_net_lable=5;

    break;

    case 5:
	   

       HAL_UART_Transmit(&huart2, "AT+TCMQTTCONN=1,5000,240,0,1\r\n", strlen("AT+TCMQTTCONN=1,5000,240,0,1\r\n"), 5000);//开始连接
       HAL_Delay(1000);
	   HAL_Delay(1000);
	   HAL_Delay(1000);
	   HAL_Delay(1000);
	   run_t.wifi_config_net_lable=0x06;
	break;

	case 6:
       HAL_UART_Transmit(&huart2, "AT+TCMQTTCONN=1,5000,240,0,1\r\n", strlen("AT+TCMQTTCONN=1,5000,240,0,1\r\n"), 5000);//开始连接
	  run_t.wifi_config_net_lable=0xff;
       esp8266data.esp8266_login_cloud_success=1;
	break;
	  

   
   }
    free(device_massage);
}
/****************************************************************************************************
	**
	*Function Name:void Wifi_SoftAP_Config_Handler(void)
	*Function: tensent cloud configuration softAP to connector WIFI
	*Input Ref: 
	*Return Ref:NO
	*
****************************************************************************************************/
void Wifi_SoftAP_Config_Handler(void)
{
     uint8_t *device_massage;
    

    device_massage = (uint8_t *)malloc(128);

  // InitWifiModule();

   switch (run_t.wifi_config_net_lable)
  {

     case wifi_set_cwmode:
    	    WIFI_IC_ENABLE();
         	HAL_UART_Transmit(&huart2, "AT+CWMODE=2\r\n", strlen("AT+CWMODE=2\r\n"), 5000);
        	HAL_Delay(200);
			run_t.wifi_config_net_lable =wifi_set_softap;

	 break;

	  case wifi_set_softap:
            WIFI_IC_ENABLE();
			
			////HAL_UART_Transmit(&huart2, device_massage, strlen((const char *)device_massage), 5000);
	         sprintf((char *)device_massage, "AT+TCPRDINFOSET=1,\"%s\",\"%s\",\"%s\"\r\n", PRODUCT_ID, DEVICE_SECRET,DEVUICE_NAME);
			 // HAL_UART_Transmit(&huart2, device_massage, strlen((const char *)device_massage), 5000);
             usart2_flag = at_send_data(device_massage, strlen((const char *)device_massage));
	  		HAL_Delay(200);
	        if(usart2_flag !=0)
					run_t.wifi_config_net_lable=wifi_set_tcdevreg;
			else
				run_t.wifi_config_net_lable=wifi_set_softap;
	  break;


	 case wifi_set_tcdevreg://dynamic register
		 HAL_UART_Transmit(&huart2, "AT+TCDEVREG\r\n", strlen("AT+TCDEVREG\r\n"), 5000); //动态注册 
	     HAL_Delay(1000);
	     run_t.wifi_config_net_lable=wifi_set_tcsap;

	 break;


	 case wifi_set_tcsap:
	  	    sprintf((char *)device_massage, "AT+TCSAP=\"%s\"\r\n",DEVUICE_NAME);
            usart2_flag = at_send_data(device_massage, strlen((const char *)device_massage));
			
	  break;

	 
	   
  }

  

    free(device_massage);
}

/****************************************************************************************************
**
*Function Name:void SmartPhone_SmartConifig_LinkTengxunCloud(void)
*Function: dy
*Input Ref: 
*Return Ref:NO
*
****************************************************************************************************/
void SmartPhone_SmartConfig_LinkTengxunCloud(void)
{
   
    uint8_t *device_massage;

    device_massage = (uint8_t *)malloc(128);

	if(esp8266data.esp8266_smartphone_flag ==1){
		
        if(esp8266data.esp8266_timer_1s >10){
		   esp8266data.esp8266_timer_1s=0;
		   esp8266data.esp8266_smartphone_flag=0; //return this function

	      sprintf((char *)device_massage, "AT+TCPRDINFOSET=1,\"%s\",\"%s\",\"%s\"\r\n", PRODUCT_ID, DEVICE_SECRET,DEVUICE_NAME);
	      HAL_UART_Transmit(&huart2, device_massage, strlen((const char *)device_massage), 5000);
          HAL_Delay(1000);

		  esp8266data.esp8266_dynamic_reg_flag=1;
		   esp8266data.esp8266_link_cloud_flag=1;
		   esp8266data.esp8266_timer_link_1s=0;
           
        }
		

	}

	if(esp8266data.esp8266_dynamic_reg_flag==1){

         esp8266data.esp8266_dynamic_reg_flag=0;
		 HAL_UART_Transmit(&huart2, "AT+TCDEVREG\r\n", strlen("AT+TCDEVREG\r\n"), 5000); //动态注册 
	     HAL_Delay(1000);
		 esp8266data.esp8266_link_cloud_flag =1;
         esp8266data.esp8266_timer_link_1s=0;
		 esp8266data.rx_link_cloud_flag=1;

     }

	if(esp8266data.esp8266_link_cloud_flag==1){
		
       if(esp8266data.esp8266_timer_link_1s > 10){
	   	esp8266data.esp8266_timer_link_1s=0;
	     esp8266data.esp8266_link_cloud_flag=0xff;
	   

       HAL_UART_Transmit(&huart2, "AT+TCMQTTCONN=1,5000,240,0,1\r\n", strlen("AT+TCMQTTCONN=1,5000,240,0,1\r\n"), 5000);//开始连接
       HAL_Delay(1000);
	 
	   esp8266data.gTimer_subscription_timing=0;
        
	  

	  }
	}
 	 free(device_massage);

}

//softAP link smartPhone
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
   
    uint8_t *device_massage;

    device_massage = (uint8_t *)malloc(128);

   InitWifiModule();
    

   if(run_t.wifi_config_net_lable ==1){
       
		 if(run_t.gTimer_wifi_1s>2){
		  run_t.wifi_config_net_lable=2;
                   run_t.gTimer_wifi_1s=0;
			WIFI_IC_ENABLE();
         	HAL_UART_Transmit(&huart2, "AT+CWMODE=3\r\n", strlen("AT+CWMODE=3\r\n"), 5000);
        	HAL_Delay(1000);
			run_t.wifi_cwsap_flag =1;

		 }
	  }

	  if(run_t.wifi_cwsap_flag ==1){
	  	  run_t.wifi_cwsap_flag =0;

          sprintf((char *)device_massage, "AT+TCSAP=\"%s\"\r\n", DEVUICE_NAME);
          HAL_UART_Transmit(&huart2, device_massage, strlen((const char *)device_massage), 5000);
          HAL_Delay(1000);
         esp8266data.esp8266_smartphone_flag =1;
		 esp8266data.esp8266_timer_1s=0;
	  }


   

	free(device_massage);

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
   
    uint8_t *device_submassage;

    device_submassage = (uint8_t *)malloc(128);

	if(esp8266data.esp8266_smartphone_flag ==1){
		 
		
        if(esp8266data.esp8266_timer_1s >20){
		
		   esp8266data.esp8266_timer_1s=0;
		   esp8266data.esp8266_smartphone_flag=0; //return this function

	      sprintf((char *)device_submassage, "AT+TCPRDINFOSET=1,\"%s\",\"%s\",\"%s\"\r\n", PRODUCT_ID, DEVICE_SECRET,DEVUICE_NAME);
	      HAL_UART_Transmit(&huart2, device_submassage, strlen((const char *)device_submassage), 5000);
          HAL_Delay(1000);

		  esp8266data.esp8266_dynamic_reg_flag=1;
            esp8266data.esp8266_timer_dynamic_1s=0;
		 
        }
		

	}

	if(esp8266data.esp8266_dynamic_reg_flag==1){
        
         if(esp8266data.esp8266_timer_dynamic_1s > 2){
            esp8266data.esp8266_timer_dynamic_1s=0;
         esp8266data.esp8266_dynamic_reg_flag=0;
		 HAL_UART_Transmit(&huart2, "AT+TCDEVREG\r\n", strlen("AT+TCDEVREG\r\n"), 5000); //动态注册 
	     HAL_Delay(100);
		 esp8266data.esp8266_link_cloud_flag =1;
         esp8266data.esp8266_timer_link_1s=0;
           esp8266data.rx_link_cloud_flag =1;
            
         }
     }

	if(esp8266data.esp8266_link_cloud_flag==1){
		
       if(esp8266data.esp8266_timer_link_1s > 6){
	   	esp8266data.esp8266_timer_link_1s=0;
	     esp8266data.esp8266_link_cloud_flag=0;
	      

       HAL_UART_Transmit(&huart2, "AT+TCMQTTCONN=1,5000,240,0,1\r\n", strlen("AT+TCMQTTCONN=1,5000,240,0,1\r\n"), 5000);//开始连接
       HAL_Delay(1000);
	//   esp8266data.esp8266_login_cloud_success=1;
	   esp8266data.gTimer_subscription_timing=0;
	

	  }
	}
 	 free(device_submassage);

}






