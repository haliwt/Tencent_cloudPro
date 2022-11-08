#ifndef __ESP8266_H_
#define __ESP8266_H_
#include "main.h"

#define SSID                    "YUYIJIA_S03"
#define PASSWD                  "12345678"

#define PRODUCT_ID              "XE4FRQT2DP" //production ID 
#define DEVUICE_NAME            "UYIJIA_S03"      //device Name:
#define DEVICE_SECRET           "YJuZKqu9cwb2Xd87Ul/0Jg==" //Secret key 
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
	unsigned char esp8266_timer_1s;
}ESP8266DATATypedef;

extern ESP8266DATATypedef esp8266data;



void Wifi_Link_SmartPhone_Fun(void);
void SmartPhone_LinkTengxunCloud(void);






#endif 
