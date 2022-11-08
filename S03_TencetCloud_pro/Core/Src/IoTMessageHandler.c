#include "loTMSG.h"
#include <string.h>
#include <stdlib.h>



#include "gpio.h"
#define LOT_Debug 0

uint8_t TCMQTTRCVPUB[256];
uint8_t Sub_Topic[128];
uint8_t Sub_Data[128];

//处理腾讯云下发的数据
void loTMessageHandler(void)
{
    char *pub_buf;
    unsigned int i = 0;

    if (TCMQTTRCVPUB != 0)
    {
        //分离出Topic
        pub_buf = strstr((const char *)TCMQTTRCVPUB, "$thing/down/property");
        if (pub_buf != NULL)
        {
            while (*pub_buf != '\"')
            {
                Sub_Topic[i] = *pub_buf++;
                i++;
            }
            i = 0;
#if LOT_Debug
            printf("Topic=%s\r\n", Sub_Topic);
#endif
        }
        //分离出数据
        pub_buf = strstr((const char *)TCMQTTRCVPUB, "{\"method\":\"control\"");
        if (pub_buf != NULL)
        {
            while (*pub_buf != '\n')
            {
                Sub_Data[i] = *pub_buf++;
                i++;
            }
            //去掉结尾的双引号
            Sub_Data[i - 2] = '\0';
#if LOT_Debug
            printf("Data=%s\r\n", Sub_Data);
#endif

            i = 0;
        }
    }

    memset(TCMQTTRCVPUB, 0, sizeof(TCMQTTRCVPUB));
}

void cJsonMessageHandler(uint8_t *cJsonDATA)
{
    if (cJsonDATA != NULL)
    {
        cJSON *_JsonRoot = cJSON_Parse((const char *)cJsonDATA);
        if (_JsonRoot == NULL)
            goto __cJSON_Delete;

        cJSON *cJSON_params = cJSON_GetObjectItem(_JsonRoot, "params");
        if (cJSON_params == NULL)
            goto __cJSON_Delete;

        cJSON *cJSON_led1 = cJSON_GetObjectItem(cJSON_params, "led1");
#if LOT_Debug
        printf("led1=%d\r\n", cJSON_led1->valueint);

#endif
//        if (cJSON_led1->valueint == 1)
//             //HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
//        else
//           // HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
    __cJSON_Delete:
        cJSON_Delete(_JsonRoot);
        memset(Sub_Data, 0, sizeof(Sub_Data));
    }
}

