#ifndef __SUBSCRIPTION_H_
#define __SUBSCRIPTION_H_
#include "main.h"

typedef enum _subSignal{ /* enumeration for CParser signals */
   OPEN_SIG=0x01, 
   STATE_SIG, 
   PTC_SIG,
   SONIC_SIG,
   ANION_SIG,
   TEMP_SIG,
   FIND_SIG,
   HUM_SIG,
   NOWTEMP_SIG
}subSignal;

typedef enum _subState {                     /* enumeration for CParser states */
   STANDBY, START, WORKS, RUN,END
}subState;

typedef struct Subscription Subscription1;  
struct Subscription1 {
   subState substate__;                /* the scalar state-variable */
   uint8_t commentCtr__;                 /* comment character counter */
   /* ... */                          /* other CParser1 attributes */
};

#define SubscriptionInit(me_) \
   ((me_)->commentCtr__ = 0, SubscriptionTran(me_, STANDBY)) 

//void SubscriptionDispatch(Subscription1 *me, unsigned const sig);

#define SubscriptionTran(me_, target_)    ((me_)->state__ = target_)

#define SubscriptionGetCommentCtr(me_) ((me_)->commentCtr__)

void Subscription_Handler(void);




typedef enum _JSONTYPE {
   JSNONE = -1,
   JSSTRING = 0,
   JSOBJECT,
   JSARRAY,
   JSNUMBER,
   JSBOOLEAN,
   JSNULL,
   JSTYPEMAX
}JSONTYPE;

extern uint8_t TCMQTTRCVPUB[256];
extern uint8_t Sub_Topic[128];
extern uint8_t Sub_Data[128];

void Receive_Data_FromCloud_Data(int type, char *str);
void Wifi_Rx_InputInfo_Handler(void);


void Subscriber_Data_FromCloud_Handler(void);
void Subscribe_Rx_Interrupt_Handler(void);
#endif 