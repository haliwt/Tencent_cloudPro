#ifndef __SUBSCRIPTION.H_
#define __SUBSCRIPTION.H_
#include "main.h"

typedef enum subSignal { /* enumeration for CParser signals */
   OPEN_SIG=0x01, 
   STATE_SIG, 
   PTC_SIG,
   SONIC_SIG,
   ANION_SIG,
   TEMP_SIG,
   FIND_SIG,
   HUM_SIG,
   NOWTEMP_SIG
};
typedef enum subState {                     /* enumeration for CParser states */
   STANDBY, START, WORKS, RUN,END
};
typedef struct Subscription Subscription1;  
struct Subscription1 {
   enum subState substate__;                /* the scalar state-variable */
   uint8_t commentCtr__;                 /* comment character counter */
   /* ... */                          /* other CParser1 attributes */
};

#define SubscriptionInit(me_) \
   ((me_)->commentCtr__ = 0, SubscriptionTran(me_, STANDBY)) 

//void SubscriptionDispatch(Subscription1 *me, unsigned const sig);

#define SubscriptionTran(me_, target_)    ((me_)->state__ = target_)

#define SubscriptionGetCommentCtr(me_) ((me_)->commentCtr__)

void Subscription_Handler(void);




typedef enum JSONTYPE {
   JSNONE = -1,
   JSSTRING = 0,
   JSOBJECT,
   JSARRAY,
   JSNUMBER,
   JSBOOLEAN,
   JSNULL,
   JSTYPEMAX
};

extern uint8_t TCMQTTRCVPUB[256];
extern uint8_t Sub_Topic[128];
extern uint8_t Sub_Data[128];

void Receive_Data_FromCloud_Data(int type, char *str);
void Wifi_Rx_InputInfo_Handler(void);

void Subscriber_Data_FromCloud_Handler();

#endif 