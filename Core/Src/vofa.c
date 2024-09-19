/*
 * vofa.c
 *
 *  Created on: Aug 23, 2024
 *      Author: GAOTIANHAO
 */
#include "vofa.h"

Example a={0};

//分割线
//================VOFA HEX库=====================
//================VOFA HEX库=====================

//================VOFA STR库=====================
uint8_t g_vofa_rx_buffer[VOFA_FRAME_RECEIVE_MAX_LENGTH]={0};
uint8_t g_vofa_frame_received=0;
uint8_t g_vofa_frame_size=0;
uint8_t g_vofa_frame_byte=0;
uint8_t g_vofa_rx_index=0;

uint8_t _calASCII(char num){
	return (num-ASCII_TRANS);
}

uint8_t _calculateData(char *data,uint16_t dataLenth,float *sum){
	uint8_t integer_end=dataLenth-(VOFA_DATA_DECIMAL_LEN+1);//整数位数
	uint8_t decimal_start=dataLenth-VOFA_DATA_DECIMAL_LEN;//小数起始位数组下标
	uint8_t index=0;
	if(dataLenth>=4){
		for(index=0;index<integer_end;index++){
			*sum+=(_calASCII(data[index])*pow(10.0,integer_end-index-1));//幂次与下标和整数位长度有关
		}
		for(index=0;index<2;index++){
			*sum+=(_calASCII(data[decimal_start+index])*pow(0.1,index+1));//从小数起始位遍历两位
		}
		return VOFA_SET_OK;//计算成功
	}
	return VOFA_SET_NO;//计算失败
}

//移植请修改下面的函数
//后续可以改成链表遍历+泛型的方式，传入链表对链表遍历实现查询VOFA对象
uint8_t _setVofaProtocol(float data,char *str){
	char p1[]="P1";//移植请修改这里，下面同理
	char p2[]="P2";


	if(strcmp(str,p1)==0){//移植请修改这里，下面同理
		//绑定值
		a.v1=(uint16_t)data;//移植请修改这里，下面同理
		printf("[VOFA]P1=%.4f\n",a.v1);
		return VOFA_SET_OK;
	}
	else if(strcmp(str,p2)==0){
		//绑定值
		a.v2=(uint16_t)data;
		printf("[VOFA]P2=%.4f\n",a.v2);
		return VOFA_SET_OK;
	}

	else{
		return VOFA_SET_NO;
	}
}

uint8_t _getVofaData(uint16_t frameLenth){//frameLenth长度不包括协议的结尾符号
	uint16_t index=0;
	uint16_t dataLenth=0;
	uint16_t strLenth=0;
	char *rawdata=NULL;
	char *rawstr=NULL;
	float data=0;//最终解算得到的数据
//	printf("OK\n");
	for(index=0;index<frameLenth;index++) // 查找等号的位置，不包含结尾的符号
	{
		if(g_vofa_rx_buffer[index]==VOFA_FRAME_STR_FRONT){
			strLenth=index+1;//+1是为了变成字符串，以\0结尾
			dataLenth=frameLenth-index-1;//不包含结尾的数据结束符号
			break ;
		}
	}
	rawdata=(char *)malloc(sizeof(char)*dataLenth);
	rawstr=(char *)malloc(sizeof(char)*strLenth);
	if(rawdata != NULL&&rawstr != NULL){
		memcpy(rawstr,g_vofa_rx_buffer,strLenth-1);
		rawstr[strLenth-1]='\0';//变成字符串
		memcpy(rawdata,(g_vofa_rx_buffer+strLenth),dataLenth);
		if(_calculateData(rawdata,dataLenth,&data)){//如果计算成功
			if(_setVofaProtocol(data,rawstr)){//这里没有做error_handler
				free(rawdata);
				free(rawstr);
				return VOFA_SET_OK;
			}
		}
		else{
			free(rawdata);
			free(rawstr);
			return VOFA_SET_NO;
		}
	}
	else{
		printf("[VOFA]vofa malloc fail\n");
		return VOFA_SET_NO;
	}

}

void Vofa_Test(){
	if(g_vofa_frame_received){
		g_vofa_frame_received=0;
//		printf("%d\n",g_vofa_frame_size);
		if(_getVofaData(g_vofa_frame_size)){//如果有结尾的符号，这里传入的size要-1
			printf("[VOFA]vofa set success\n");
		}
		else{
			printf("[VOFA]vofa set fail\n");
		}
	}
}

//================VOFA STR库=====================
