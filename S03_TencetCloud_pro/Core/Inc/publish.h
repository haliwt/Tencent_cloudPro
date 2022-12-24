#ifndef __PUBLISH_H_
#define __PUBLISH_H_
#include "main.h"

enum Signal {                   /* enumeration for CParser signals */
   CHAR_SIG, STAR_SIG, SLASH_SIG
};
enum State {                     /* enumeration for CParser states */
   CODE, SLASH, COMMENT, STAR
};
typedef struct Publish Publish;  
struct Publish {
   enum State state__;                /* the scalar state-variable */
   long commentCtr__;                 /* comment character counter */
   /* ... */                          /* other CParser1 attributes */
};

#define CParser1Init(me_) \
   ((me_)->commentCtr__ = 0, CParser1Tran(me_, CODE)) 

void CParser1Dispatch(Publish *me, unsigned const sig);

#define CParser1Tran(me_, target_)    ((me_)->state__ = target_)

#define CParser1GetCommentCtr(me_) ((me_)->commentCtr__)


void Publish_Data_ToCloud_Handler(void);



#endif 