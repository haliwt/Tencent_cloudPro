#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mqtt_iot.h"
#include "esp8266.h"
#include "run.h"
#include "usart.h"


#define TOKEN_ID      "123"


//char *tx_data={"AT+TCMQTTPUB=\"$thing/up/property/EHQB1P53IH/UYIJIA01-a0005\",0,\"{\"method\":\"report\"\,\"clientToken\":\"up01\"\,\"params\":\"{\"open\":1\,\"temperature\":26}}\"\r\n"};
char *tx_data={"AT+TCMQTTPUB=\"$thing/up/property/EHQB1P53IH/UYIJIA01-a0005\",0,\"{\\\"method\\\":\\\"report\\\"\\\,\\\"clientToken\\\":\\\"up01\\\"\\,\\\"params\\\":{\\\"open\\\":1\\,\\\"temperature\\\":26}}\"\r\n"};

/* MAX size of client ID */
#define MAX_SIZE_OF_CLIENT_ID                 (80)

/* MAX size of product ID */
#define MAX_SIZE_OF_PRODUCT_ID                (10)

/* MAX size of product secret */
#define MAX_SIZE_OF_PRODUCT_SECRET            (32)

/* MAX size of device name */
#define MAX_SIZE_OF_DEVICE_NAME                (48)

/* MAX size of device secret */
#define MAX_SIZE_OF_DEVICE_SECRET             (64)




static void property_report(void);

static void property_topic_publish(void);

static void property_report_Temp_Humidity(void);


#ifdef AUTH_MODE_CERT
static char sg_cert_file[PATH_MAX + 1];  // full path of device cert file
static char sg_key_file[PATH_MAX + 1];   // full path of device key file
#endif

typedef enum {
    eCOMMON_DEV = 0,  // common dev
    eGW_DEV     = 1,  // Gateway dev
    eGW_SUB_DEV = 2,  // sub dev of Gateway
    eDEFAULT_DEV=3,
    eTONKEN=4
} eDevType;

typedef struct {
    char product_id[MAX_SIZE_OF_PRODUCT_ID + 1];
    char device_name[MAX_SIZE_OF_DEVICE_NAME + 1];
    char client_id[MAX_SIZE_OF_CLIENT_ID + 1];

   

    eDevType dev_type;
} DeviceInfo;





typedef struct {
    
    bool     open;
	bool     sonic;
	bool     state ;
	bool     ptc;
	bool     anion;
    
	uint8_t  find;
	uint8_t  nowtemperature;
    uint8_t  temperature;
    uint8_t  humidity;

    
  
} serviceInfo;

static serviceInfo   sg_devInfo;
static serviceInfo    sg_info;
static DeviceInfo    sg_equipInfo;

// led attributes, corresponding to struct LedInfo
static char *sg_property_name[] = {"opne", "sonic", "find", "nowtemperature","state","ptc","Anion","temperature","Humidity"};
void Mqtt_Value_Init(void)
{
    
   	sg_info.open=1;
    sg_info.state=1;
    sg_info.ptc=1; 
    sg_info.anion=1;

	sg_info.find=54;
   	sg_info.nowtemperature=27;
    sg_info.temperature=36;
    sg_info.humidity=64;



}

static void property_topic_publish(void)
{
    char topic[256] = {0};
    int  size;

    size = snprintf(topic, 256, "AT+TCMQTTPUB=\"$thing/up/property/%s/%s\",0,", PRODUCT_ID,DEVUICE_NAME);



   at_send_data(topic, size);
 
   
}



static void property_report(void)
{
    char       message[256]    = {0};
    int        message_len     = 0;


   

   message_len = snprintf(message, sizeof(message),"\"{\\\"method\\\":\\\"report\\\"\\,\\\"clientToken\\\":\\\"%s\\\"\\,\\\"params\\\":{\\\"open\\\":%d\\,\\\"Anion\\\":%d\\,\\\"ptc\\\":%d\\,\\\"sonic\\\":%d\\,\\\"find\\\":%d\\,\\\"state\\\":%d\\}}\"\r\n",
                             TOKEN_ID,sg_info.open,sg_info.anion,sg_info.ptc,sg_info.sonic,sg_info.find,sg_info.state);
                               
 
	at_send_data(message, message_len);
   
}

static void property_report_Temp_Humidity(void)
{

	   char	message[128]    = {0};
	   int	message_len	  = 0;
	
	
	  
	
	  message_len = snprintf(message, sizeof(message),"\"{\\\"method\\\":\\\"report\\\"\\,\\\"clientToken\\\":\\\"%s\\\"\\,\\\"params\\\":{\\\"nowtemperature\\\":%d\\,\\\"temperature\\\":%d\\,\\\"Humidity\\\":%d}}\"\r\n",
								TOKEN_ID,sg_info.nowtemperature,sg_info.temperature,sg_info.humidity);
								  
	
	   at_send_data(message, message_len);




}


//static void property_get_status(void *pClient)
//{
//    char       message[256]        = {0};
//    int        message_len         = 0;
//    static int sg_get_status_index = 0;

//    sg_get_status_index++;
//    message_len = snprintf(message, sizeof(message), "{\"method\":\"get_status\", \"clientToken\":\"%s-%d\"}",
//                               sg_devInfo.product_id, sg_get_status_index);
//    property_topic_publish(message, message_len);
//}

/********************************************************************************
	*
	*Function Name:void IOT_MQTT_Publish(pClient, topic, &pubParams)
	*Function :send data to tencent cloud 
	*Input Ref: pClient ->client data array ,topic->tencent register topic name,
	*           &pubParams -> topic of reference
	*Return Ref:
	*
********************************************************************************/
void IOT_MQTT_Publish(void)
{
   
   property_topic_publish();
   property_report();

   
   // property_topic_publish(); 
   // property_report_Temp_Humidity();
    
    
}


#if 0
AT+TCMQTTPUB="$thing/up/property/4ZWKRTZ28S/UYIJIA_S01_T",0,
"{\"method\":\"report\"\,\"clientToken\":\"123\"\,\"params\":{\"display_temperature\":26\,\"humidity_value\":82\,\"display_timing\":1 }} "

AT+TCMQTTPUB="$thing/up/property/4ZWKRTZ28S/UYIJIA_S01_T",0,"{\"method\":\"report\"\,\"clientToken\":\"123\"\,\"params\":{\"display_temperature\":26\,\"humidity_value\":82\,\"display_timing\":1 }} "


#endif 


