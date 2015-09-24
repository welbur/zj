

#ifndef __IWDG_H
#define __IWDG_H

#include "Global_Config.h"

#if WATCHDOG_ENABLE

void IWDG_Configuration(void);
u32 GetLSIFrequency(void);

#endif

#endif


