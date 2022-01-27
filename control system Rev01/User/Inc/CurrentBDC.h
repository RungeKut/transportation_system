#ifndef CURRENT_BDC_H
#define CURRENT_BDC_H

#include "main.h"

extern uint32_t BDC_mAmp;

/* BEGIN Prototypes */
void CurrentBDC_StartUp(void);
void CurrentBDC_GET(void);
/* END Prototypes */

#endif /* CURRENT_BDC_H */
