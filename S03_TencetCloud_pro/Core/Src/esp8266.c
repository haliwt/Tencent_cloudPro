#include "esp8266.h"
#include "usart.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "run.h"
#include "mqtt_iot.h"
#include "wifi_fun.h"
#include "cmd_link.h"


ESP8266DATATypedef esp8266data;

 uint8_t *sub_buf;

char *CloudInfo="+TCMQTTCONN:OK";
char *usart2_tx;

uint8_t usart2_flag;








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
void InitWifiModule(void)
{
	
	
		
			
			WIFI_IC_ENABLE();
	
	
			at_send_data("AT+RST\r\n", strlen("AT+RST\r\n"));
			HAL_Delay(100);
			at_send_data("AT+RST\r\n", strlen("AT+RST\r\n"));
		
			HAL_Delay(100);
			
		

}



/****************************************************************************************************
	**
	*Function Name:void Wifi_Link_SmartConfig_Fun(void)
	*Function: 
	*Input Ref: 
	*Return Ref:NO
	*
****************************************************************************************************/
#if 0
void Wifi_Link_SmartConfig_Handler(void)
{
       uint8_t *device_massage;
  

    device_massage = (uint8_t *)malloc(128);

   InitWifiModule();
    
   switch(run_t.wifi_config_net_lable){

   	case 1:

   			WIFI_IC_ENABLE();
         	HAL_UART_Transmit(&huart2, "AT+CWMODE=2\r\n", strlen("AT+CWMODE=2\r\n"), 5000);
        	HAL_Delay(1000);
			HAL_UART_Transmit(&huart2, "AT+CIPMUX=1\r\n", strlen("AT+CIPMUX=1\r\n"), 5000);
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
	   
	   HAL_Delay(1000);

       HAL_UART_Transmit(&huart2, "AT+TCMQTTCONN=1,5000,240,0,1\r\n", strlen("AT+TCMQTTCONN=1,5000,240,0,1\r\n"), 5000);//开始连接
       
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
#endif 
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


   switch (run_t.wifi_config_net_lable)
  {

    case wifi_set_restor:
           InitWifiModule();
		   HAL_Delay(1000);
           run_t.wifi_config_net_lable =wifi_set_cwmode;
	break;


	 case wifi_set_cwmode:
    	    WIFI_IC_ENABLE();
         	HAL_UART_Transmit(&huart2, "AT+CWMODE=3\r\n", strlen("AT+CWMODE=3\r\n"), 5000);
        	HAL_Delay(1000);
			HAL_Delay(1000);
			//HAL_UART_Transmit(&huart2, "AT+CIPMUX=1\r\n", strlen("AT+CIPMUX=1\r\n"), 5000);
			run_t.wifi_config_net_lable =wifi_set_softap;
			run_t.randomName[0]=HAL_GetUIDw0();
		

	 break;

	  case wifi_set_softap:
            WIFI_IC_ENABLE();
			
            sprintf((char *)device_massage, "AT+TCPRDINFOSET=1,\"%s\",\"%s\",\"UYIJIA01-%d\"\r\n", PRODUCT_ID, DEVICE_SECRET,run_t.randomName[0]);
			usart2_flag = at_send_data(device_massage, strlen((const char *)device_massage));
	  		HAL_Delay(1000);
            HAL_Delay(1000);
			 HAL_Delay(1000);
	        
			run_t.wifi_config_net_lable=wifi_set_tcdevreg;
		
	  break;


	 case wifi_set_tcdevreg://dynamic register
		 HAL_UART_Transmit(&huart2, "AT+TCDEVREG\r\n", strlen("AT+TCDEVREG\r\n"), 0xffff); //动态注册 
	      HAL_Delay(1000);
		 HAL_Delay(1000);
		HAL_Delay(1000);
		HAL_Delay(1000);

	  
	     run_t.wifi_config_net_lable=wifi_set_tcsap;

	 break;


	 case wifi_set_tcsap:
	 
            HAL_Delay(1000);
		    HAL_Delay(1000);
			HAL_Delay(1000);
		    HAL_Delay(1000);
	        sprintf((char *)device_massage, "AT+TCSAP=\"UYIJIA01-%d\"\r\n",run_t.randomName[0]);
            usart2_flag = at_send_data(device_massage, strlen((const char *)device_massage));
			 HAL_Delay(1000);
             HAL_Delay(1000);
			 wifi_t.soft_ap_config_flag =1;
			 esp8266data.rx_link_cloud_flag =1; //enable usart2 receive wifi  data
			 UART2_DATA.UART_Cnt=0;
			 run_t.wifi_config_net_lable=0xff;
			
	 break;

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
void SmartPhone_LinkTencent_Cloud(void)
{
   
    uint8_t *device_submassage;

    device_submassage = (uint8_t *)malloc(128);


	if(esp8266data.soft_ap_config_success==1){

       esp8266data.soft_ap_config_success=0;
	   HAL_UART_Transmit(&huart2, "AT+TCMQTTCONN=1,5000,240,0,1\r\n", strlen("AT+TCMQTTCONN=1,5000,240,0,1\r\n"), 5000);//开始连接
       HAL_Delay(1000);
       HAL_Delay(1000);
       SendWifiData_To_Cmd(1);//To tell display panel wifi be connetor to tencent cloud is success
	 
     }
	
 	 free(device_submassage);

}

void SmartPhone_TryToLink_TencentCloud(void)
{
    esp8266data.rx_link_cloud_flag =1; //enable usart2 receive wifi  data
    UART2_DATA.UART_Cnt=0;
	wifi_t.soft_ap_config_flag =0;
    HAL_UART_Transmit(&huart2, "AT+TCMQTTCONN=1,5000,240,0,1\r\n", strlen("AT+TCMQTTCONN=1,5000,240,0,1\r\n"), 5000);//开始连接
	HAL_Delay(1000);
    HAL_Delay(1000);
	   
}

void wifi_Disconnect_Fun(void)
{

  HAL_UART_Transmit(&huart2, "AT+TCMQTTDISCONN\r\n", strlen("AT+TCMQTTDISCONN\r\n"), 5000);//开始连接

}

void Get_BeiJing_Time_Cmd(void)
{

  HAL_UART_Transmit(&huart2, "AT+CIPSNTPCFG=1,8,\"cn.ntp.org.cn\",\"ntp.sjtu.edu.cn\"\r\n", strlen("AT+CIPSNTPCFG=1,800,\"cn.ntp.org.cn\",\"ntp.sjtu.edu.cn\"\r\n"), 5000);//开始连接

}

void Get_Beijing_Time(void)
{

   HAL_UART_Transmit(&huart2, "AT+CIPSNTPTIME?\r\n", strlen("AT+CIPSNTPTIME?\r\n"), 3000);//开始连接


}

void Wifi_Break_Link(void)
{

  HAL_UART_Transmit(&huart2, "AT+CWQAP\r\n", strlen("AT+CWQAP\r\n"), 3000);


}
