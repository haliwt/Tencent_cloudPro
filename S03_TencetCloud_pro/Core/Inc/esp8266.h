#ifndef __ESP8266_H_
#define __ESP8266_H_
#include "main.h"
#include "gpio.h"

#define SSID                    "UYIKIA"
#define PASSWD                  "20329263"


#define TOPIC                  "open"

#define TOPIC_VALUE            1
//#define SMARTCONFIG      			0


#define PRODUCT_ID              "EHQB1P53IH" //production ID 
#define DEVUICE_NAME            "UYIJIA01-afcc8"      //device Name:
#define DEVICE_SECRET           "5xdQMgx7ZHznYyQK6wvdmolc"//"5xdQMgx7ZHznYyQK6wvdmolc"  //Secret key 
#define PRODUCT_REGION          "ap-guangzhou" //filed name 

#define WIFI_IC_ENABLE()         HAL_GPIO_WritePin(WIFI_EN_GPIO_Port,WIFI_EN_Pin,GPIO_PIN_SET)
#define WIFI_IC_DISABLE()        HAL_GPIO_WritePin(WIFI_EN_GPIO_Port,WIFI_EN_Pin,GPIO_PIN_RESET)


typedef enum _esp8266_para{

	wifi_set_restor=0x00,
	wifi_set_cwmode,
	wifi_set_softap,
	wifi_set_tcdevreg,
	wifi_set_tcsap,
	wifi_login_tencent


}esp8266_para;


typedef struct ESP8266_DATA
{
    uint8_t data[512];
    uint8_t  data_size;
    uint8_t flag;
	uint8_t wifi_RunState;
	uint8_t esp8266_smartphone_flag;
	uint8_t esp8266_link_cloud_flag;
	uint8_t soft_ap_config_success;
	uint8_t esp8266_login_cloud_success;
    uint8_t publish_flag;
	uint8_t subscribe_cloud_success;
	
	
	uint8_t esp8266_dynamic_reg_flag;
    uint8_t subscribe_flag;
	uint8_t subscribe_rx_flag;
    uint8_t subscribe_rxCloud_flag;
	uint8_t esp8266_data_rx_success;
	uint8_t rx_data_success ;
	uint8_t  rx_counter ;
	uint8_t  rx_data_state;
	uint8_t getCloudValue_unit ;
	uint8_t getCloudValue_decade;
	uint8_t rx_data_len;
	uint8_t rx_data_name_len;
	uint8_t rx_link_cloud_flag;
    uint8_t cmp_flag;
	uint8_t send_tencent_cloud_data_lable;
    uint8_t subscrible_receive_data_label;
	



	uint8_t gTimer_subscription_timing;

 
	uint8_t gTimer_publish_timing;
	uint8_t gTimer_publish_dht11;
	
}ESP8266DATATypedef;



extern ESP8266DATATypedef esp8266data;
void Wifi_Link_SmartConfig_Handler(void);

void SmartPhone_LinkTencent_Cloud(void);

uint8_t at_send_data(uint8_t *pdata, uint16_t len);
void Wifi_Rx_InputInfo_Handler(void);

void Wifi_SoftAP_Config_Handler(void);
void SmartPhone_TryToLink_TencentCloud(void);
void wifi_Disconnect_Fun(void);


#endif 
