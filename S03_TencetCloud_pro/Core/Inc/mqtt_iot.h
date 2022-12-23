
#ifndef __MQTT_IOT_H_
#define __MQTT_IOT_H_
#include "main.h"



/**
 * @brief MQTT Quality of Service level
 *
 * Check MQTT spec for QoS define
 */
typedef enum _QoS {
    QOS0 = 0,  // At most once delivery
    QOS1 = 1,  // At least once delivery, PUBACK is required
    QOS2 = 2   // Exactly once delivery. NOT supported currently
} QoS;








/**
 * @brief Get the status of loop thread
 *
 * @param pClient       handle to MQTT client
 * @param exit_code     exit code of the thread
 * @return true= thread running, false = thread quit
 */


//int IOT_MQTT_Publish(void *pClient, char *topicName, PublishParams *pParams);

void Mqtt_Value_Init(void);

void MqttData_Publish_State(void);

void MqttData_Publis_ReadTempHum(uint8_t tempvalue,uint8_t humvalue);

void MqttData_Publis_SetFan(uint8_t fan);

void MqttData_Publis_SetTemp(uint8_t temp);



/**
 * @brief Active triggering of mqtt reconnection
 *
 * @param pClient       handle to MQTT client
 * @return error code of last IOT_MQTT_Construct operation
 */


#endif /* QCLOUD_IOT_EXPORT_MQTT_H_ */
