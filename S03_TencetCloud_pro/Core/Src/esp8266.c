#include "esp8266.h"
#include "usart.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "run.h"
#include "json_parser.h"
#include "iot_tencent.h"
#include "mqtt_iot.h"
#define esp8266_Debug 				  0
#define RTOS_Debug   				 0

ESP8266DATATypedef esp8266data;

 uint8_t *sub_buf;

 char *CloudInfo="+TCMQTTCONN:OK";
char *usart2_tx;




static void InitWifiModule(void);


static uint8_t wifi_inputBuf[20];

/**
 *pdata: pointer of data for send
 *len:  len of data to be sent
 *return: the len of data send success
 * @brief hal api for at data send
 */
uint8_t at_send_data(uint8_t *pdata, uint16_t len)
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

//    if(strstr((const char*)esp8266data.data,"+TCMQTTCONN:OK")){
//      esp8266data.flag=2; //esp8266data.flag =1 ->connect to tengxun cloud .
//    }
//    if(strstr((const char*)esp8266data.data,"+TCMQTTSTATE:0")){ //query MQTT order 
//     esp8266data.flag=11; // 0->MQTT已经断开,1->MQTT已经连接
//    }
    
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
	
	if(run_t.wifi_init_flag==0)
		{
			run_t.wifi_init_flag++;
		  //  HAL_UART_Abort(&huart2);
			
			WIFI_IC_ENABLE();
	
	
			run_t.gTimer_wifi_1s=0;
			at_send_data("AT+RST\r\n", strlen("AT+RST\r\n"));
			HAL_Delay(100);
			at_send_data("AT+RST\r\n", strlen("AT+RST\r\n"));
			run_t.gTimer_wifi_1s=0;
			
			
		}
		//HAL_UART_Abort(&huart2);

}

#ifdef  SMARTCONFIG
/****************************************************************************************************
**
*Function Name:void Wifi_Link_SmartConfig_Fun(void)
*Function: 
*Input Ref: 
*Return Ref:NO
*
****************************************************************************************************/
void Wifi_Link_SmartConfig_Fun(void)
{
       uint8_t *device_massage;
    static uint8_t wifi_cw=0xff,wifi_cwsap=0xff;

    device_massage = (uint8_t *)malloc(128);

   InitWifiModule();
    

   if(run_t.wifi_init_flag ==1){
    
	  if(run_t.gTimer_wifi_1s>5){
	  	  run_t.gTimer_wifi_1s=0;
		 if(wifi_cw != run_t.wifi_cwmode_flag){
		    wifi_cw = run_t.wifi_cwmode_flag;
			WIFI_IC_ENABLE();
         	HAL_UART_Transmit(&huart2, "AT+CWMODE=3\r\n", strlen("AT+CWMODE=3\r\n"), 5000);
        	HAL_Delay(500);
			run_t.wifi_cwsap_flag =1;

		 }
	  }

	  if(run_t.wifi_cwsap_flag ==1){
	  	  run_t.wifi_cwsap_flag =0;
          WIFI_IC_ENABLE();
	     // sprintf((char *)device_massage, "AT+TCSAP=\"%s\"\r\n", DEVUICE_NAME,);
         //HAL_UART_Transmit(&huart2, device_massage, strlen((const char *)device_massage), 5000);

         HAL_UART_Transmit(&huart2, "AT+CWJAP=\"UYIKIA\",\"20329263\"\r\n", strlen("AT+CWJAP=\"UYIKIA\",\"20329263\"\r\n"), 5000);
         HAL_Delay(10000);
         esp8266data.esp8266_smartphone_flag =1;
		 esp8266data.esp8266_timer_1s=0;
	  }


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
	     esp8266data.esp8266_link_cloud_flag=0;
	   

       HAL_UART_Transmit(&huart2, "AT+TCMQTTCONN=1,5000,240,0,1\r\n", strlen("AT+TCMQTTCONN=1,5000,240,0,1\r\n"), 5000);//开始连接
       HAL_Delay(1000);
	 
	   esp8266data.gTimer_subscription_timing=0;
        esp8266data.subscribe_flag=1; //Enable receive subscribe data from tencent cloud
	  

	  }
	}
 	 free(device_massage);

}
#else
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
    

   if(run_t.wifi_init_flag ==1){
       
		 if(run_t.gTimer_wifi_1s>2){
		  run_t.wifi_init_flag=2;
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
#endif 
/*******************************************************************************
**
*Function Name:void Publish_Data_ToCloud(void)
*Function: dy
*Input Ref: 
*Return Ref:NO
*
********************************************************************************/
void Publish_Data_ToCloud(void)
{
    
   if(esp8266data.esp8266_login_cloud_success==1){
          

		  if(esp8266data.gTimer_publish_timing > 20){
		  	
		  	esp8266data.gTimer_publish_timing=0;
			esp8266data.gTimer_subscription_timing=0;
            esp8266data.subscribe_flag =1; //allow subscribe enable

           IOT_MQTT_Publish();
	    
		  
		  
		}
	    if(esp8266data.gTimer_publish_timing >  10 && esp8266data.gTimer_publish_timing < 12){
             

		     MqttData_ToCloud_TempHumidity();

		}
	}
	

   // free(device_pubmassage);
}
/*******************************************************************************
**
*Function Name:void Publish_Data_ToCloud(void)
*Function: dy
*Input Ref: 
*Return Ref:NO
*
********************************************************************************/
void Subscriber_Data_FromCloud(void)
{
	uint8_t *device_massage;

    
	if(esp8266data.subscribe_flag ==1){
         device_massage = (uint8_t *)malloc(128);
	  if(esp8266data.gTimer_subscription_timing > 5){
	  	  esp8266data.gTimer_subscription_timing=0;
		

     sprintf((char *)device_massage, "AT+TCMQTTSUB=\"$thing/down/property/%s/%s\",0\r\n", PRODUCT_ID, DEVUICE_NAME);
     HAL_UART_Transmit(&huart2, device_massage, strlen((const char *)device_massage), 5000);
	 
         esp8266data.gTimer_tencent_down_1s =0;
         esp8266data.subscribe_rx_flag=1;
          esp8266data.subscribe_flag =0;
		esp8266data.subscribe_rxCloud_flag=1;
	}
      
     free(device_massage);
	}

   


}


void Parse_Cloud_Data(void)
{
    

	if(  esp8266data.subscribe_rxCloud_flag==1){

	 
        Receive_Data_FromCloud_Data(JSOBJECT,TCMQTTRCVPUB);
      
    
    }
   
	
	
	 

  }
/*******************************************************************************
**
*Function Name:void Subscribe_Rx_IntHandler(void)
*Function: interrupt USART2 receive data fun
*Input Ref: 
*Return Ref:NO
*
********************************************************************************/
void Subscribe_Rx_IntHandler(void)
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
		  
		   
		   UART2_DATA.UART_Data[esp8266data.rx_counter] = UART2_DATA.UART_DataBuf[0];
				esp8266data.rx_counter++ ;
					  
			if(UART2_DATA.UART_DataBuf[0]=='}') // 0x0A = "\n"
			{
			   
			    esp8266data.rx_data_success++;
				if(esp8266data.rx_data_success==1){
					esp8266data.rx_data_success++;
				    strcpy(TCMQTTRCVPUB, UART2_DATA.UART_Data);
				}
				esp8266data.rx_data_state=0;
				esp8266data.rx_counter=0;

			  break;
					
			}
			else{

			   esp8266data.rx_data_state=10; //=1

			}
		 
			   
		break;



        default:
			
			
	    break;
		}
 

}
//void Wifi_SubscribeCloud_Data(void)
//{
//   
//          strcpy((char *)TCMQTTRCVPUB, (const char *)UART2_DATA.UART_Data);
//          esp8266data.data_size = UART2_DATA.UART_Cnt;
//    
//            if(strstr((const char*)esp8266data.data,"params")){
//              esp8266data.subscribe_cloud_success=1;
//			   
//			  
//           }
//		   UART2_DATA.UART_Flag = 0;
//           UART2_DATA.UART_Cnt=0;
//         

//}
/*******************************************************************************
**
*Function Name:void Subscribe_Rx_IntHandler(void)
*Function: interrupt USART2 receive data fun
*Input Ref: +TCMQTTCONN:OK
*Return Ref:NO
*
********************************************************************************/
void Wifi_Rx_Input_Handler(void)
{
    
    
          strcpy((char *)esp8266data.data, (const char *)UART2_DATA.UART_Data);
          esp8266data.data_size = UART2_DATA.UART_Cnt;
    
            if(strstr((const char*)esp8266data.data,"+TCMQTTCONN:OK")){
              esp8266data.esp8266_login_cloud_success=1;
			   esp8266data.rx_link_cloud_flag=0;
			  
           }
		   UART2_DATA.UART_Flag = 0;
           UART2_DATA.UART_Cnt=0;
         
        
		   	
}


