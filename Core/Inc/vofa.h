/*
 * vofa.h
 *
 *  Created on: Aug 23, 2024
 *      Author: GAOTIANHAO
 */

//分割线
#ifndef INC_VOFA_H_
#define INC_VOFA_H_

#include "usart.h"
#include "stdlib.h"
#include <math.h>
#include <string.h>
//================VOFA HEX库=====================
typedef struct
{
	float v1;
	float v2;
}Example;

typedef struct {
	uint8_t u8;
	uint16_t u16;
	uint32_t u32;
}VOFA;
//================VOFA HEX库=====================

//================VOFA STR库=====================
#define VOFA_FRAME_STR_FRONT ('=')
#define VOFA_FRAME_STR_NUM ('.')
//#define VOFA_FRAME_STR_LAST '!'
#define ASCII_TRANS (48)
#define VOFA_FRAME_RECEIVE_MAX_LENGTH (40)
#define VOFA_SET_OK (1)
#define VOFA_SET_NO (0)

//修改上位机发送的小数位数后,请修改下面的宏,大小为小数位长度
#define VOFA_DATA_DECIMAL_LEN (4)

extern uint8_t g_vofa_rx_buffer[VOFA_FRAME_RECEIVE_MAX_LENGTH];
extern uint8_t g_vofa_frame_received;
extern uint8_t g_vofa_frame_size;
extern uint8_t g_vofa_frame_byte;
extern uint8_t g_vofa_rx_index;


void Vofa_Test();


//================VOFA STR库=====================
#endif /* INC_VOFA_H_ */
