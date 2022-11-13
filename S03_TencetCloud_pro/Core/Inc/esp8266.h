#ifndef __ESP8266_H_
#define __ESP8266_H_
#include "main.h"

#define SSID                    "UYIJIA01-a0002"
#define PASSWD                  "12345678"

#define PRODUCT_ID              "EHQB1P53IH" //production ID 
#define DEVUICE_NAME            "UYIJIA01-a0002"      //device Name:
#define DEVICE_SECRET           "5xdQMgx7ZHznYyQK6wvdmolc" //"YJuZKqu9cwb2Xd87Ul/0Jg==" //Secret key 
#define PRODUCT_REGION          "ap-guangzhou" //filed name 

#define WIFI_IC_ENABLE()         HAL_GPIO_WritePin(WIFI_EN_GPIO_Port,WIFI_EN_Pin,GPIO_PIN_SET)
#define WIFI_IC_DISABLE()        HAL_GPIO_WritePin(WIFI_EN_GPIO_Port,WIFI_EN_Pin,GPIO_PIN_RESET)


uint8_t *Esp8266GetData(void);
void Esp8266LinkAp(uint8_t *ssid,uint8_t *passwd);
void Esp8266LinkloTExplorer(void);




typedef struct ESP8266_DATA
{
    unsigned char data[512];
    unsigned int  data_size;
    unsigned char flag;
	unsigned char wifi_RunState;
	unsigned char esp8266_smartphone_flag;
	unsigned char esp8266_link_cloud_flag;
	unsigned char esp8266_login_cloud_success;
	
	
	unsigned char esp8266_dynamic_reg_flag;
    unsigned char subsription_flag;
	unsigned char esp8266_data_rx_success;
	unsigned char rx_data_success ;
	unsigned char  rx_counter ;
	unsigned char rx_data_state;
	unsigned char getCloudValue_unit ;
	unsigned char getCloudValue_decade;
	unsigned char rx_data_len;
	unsigned char rx_data_name_len;
	

	unsigned char esp8266_timer_link_1s;
	unsigned char esp8266_timer_1s;
	unsigned char gTimer_subscription_timing;
	unsigned char gTimer_tencent_down_1s;
	
}ESP8266DATATypedef;

extern ESP8266DATATypedef esp8266data;



void Wifi_Link_SmartPhone_Fun(void);


void Wifi_Link_SmartConfig_Fun(void);
void SmartPhone_SmartConfig_LinkTengxunCloud(void);



void SmartPhone_LinkTengxunCloud(void);

void Subscriber_Data_FromCloud(void);

void Subscribe_Rx_IntHandler(void);



void Publish_Data_ToCloud(void);

void Parse_Cloud_Data(void);

void Subscribe_Rx_Handler(void);




#endif 
