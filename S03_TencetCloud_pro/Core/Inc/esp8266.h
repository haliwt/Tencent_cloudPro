#ifndef __ESP8266_H_
#define __ESP8266_H_
#include "main.h"

#define SSID                    "UYIKIA"
#define PASSWD                  "20329263"


#define TOPIC                  "open"

#define TOPIC_VALUE            1
#define SMARTCONFIG      			1


#define PRODUCT_ID              "EHQB1P53IH" //production ID 
#define DEVUICE_NAME            "UYIJIA01-a0005"      //device Name:
#define DEVICE_SECRET           "5xdQMgx7ZHznYyQK6wvdmolc"//"5xdQMgx7ZHznYyQK6wvdmolc"  //Secret key 
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
    unsigned char publish_flag;
	
	
	unsigned char esp8266_dynamic_reg_flag;
    unsigned char subscribe_flag;
	unsigned char subscribe_rx_flag;
	unsigned char esp8266_data_rx_success;
	unsigned char rx_data_success ;
	unsigned char  rx_counter ;
	unsigned char rx_data_state;
	unsigned char getCloudValue_unit ;
	unsigned char getCloudValue_decade;
	unsigned char rx_data_len;
	unsigned char rx_data_name_len;
	unsigned char rx_link_cloud_flag;
    unsigned char cmp_flag;
	

	unsigned char esp8266_timer_link_1s;
	unsigned char esp8266_timer_1s;
	unsigned char gTimer_subscription_timing;
	unsigned char gTimer_tencent_down_1s;
    uint8_t esp8266_timer_dynamic_1s;
	uint8_t gTimer_publish_timing;
	
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

uint8_t at_send_data(uint8_t *pdata, uint16_t len);
void Wifi_Rx_Input_Handler();


#endif 
