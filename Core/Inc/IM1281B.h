#ifndef __IM1281B_H
#define __IM1281B_H

#include "main.h"
#include "usart.h"

typedef struct {
	float U;  // 电压
	float I;  // 电流
	float P;  // 功率
	float PF;  // 功率因数
	float F;  // 频率
} AC_PARAM;

extern AC_PARAM AC_OUT;

void ModBus_Send(void);
void ModBus_Receive();
void ModBus_Test();

#endif
