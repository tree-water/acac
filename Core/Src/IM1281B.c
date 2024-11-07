#include "IM1281B.h"

AC_PARAM AC_OUT={
		0,
		0,
		0,
		0,
		0
};//输出交流电结构体实例


//电压互感器数据帧，一次读8个寄存器，已包含CRC校验
uint8_t ModbusFrame[]={0x01,0x03,0x00,0x48,0x00,0x08,0xC4,0x1A};

/* CRC校验函数 */
uint16_t calculateCRC(uint8_t* data, uint8_t length) {
	uint16_t crc = 0xFFFF;
	for (int i = 0; i < length; i++) {
		crc ^= data[i];
		for (int j = 0; j < 8; j++) {
			if (crc & 0x0001)
				crc = (crc >> 1) ^ 0xA001;  // CRC-16 MODBUS算法
			else
				crc = crc >> 1;
		}
	}
	return crc;
}

//十六进制数据转化为十进制数据
int hextoDec(int hex)
{
	int sum=0,mul=1;
	int i,r;
	int count=0;
	do{
		r=hex%16;
		for(i=0;i<count;i++)
			mul*=16;
		mul*=r;
		sum+=mul;
		mul=1;
		count++;
	}while(hex/=16);
	return sum;

}

uint16_t combine_bytes(unsigned char high, unsigned char low) {
	return (((unsigned short)high) << 8) | low;
}

void ConvertBytesToUInt32Array(uint8_t* data, uint32_t* output) {
	for (int i = 0; i < 8; i++) {
		output[i] = ((uint32_t)data[i * 4] << 24) |
				((uint32_t)data[i * 4 + 1] << 16) |
				((uint32_t)data[i * 4 + 2] << 8) |
				(uint32_t)data[i * 4 + 3];
	}
}

void Get_AC_Param(uint8_t *frame){
	uint32_t output[8]={0};
	ConvertBytesToUInt32Array(frame, output);
	AC_OUT.U=output[0]*0.0001f;
	AC_OUT.I=output[1]*0.0001f;
	AC_OUT.P=output[2]*0.0001f;
	AC_OUT.PF=output[4]*0.001f;
	AC_OUT.F=output[7]*0.01f;
	printf("[IM1281B] CRC Success\n");
	printf("[IM1281B] U_in=%f\n", AC_OUT.U);
	printf("[IM1281B] I_in=%f\n", AC_OUT.I);
	printf("[IM1281B] P_in=%f\n", AC_OUT.P);
	printf("[IM1281B] PF_in=%f\n", AC_OUT.PF);
	printf("[IM1281B] F_in=%f\n", AC_OUT.F);
}

void ModBus_Send(void){

	HAL_UART_Transmit(&huart2, ModbusFrame, sizeof(ModbusFrame), 100);
	HAL_Delay(250);//�??????????定要延时250ms以上
}

void ModBus_Receive(){
	uint16_t receivedCRC=combine_bytes(g_uart_rx_buffer[36], g_uart_rx_buffer[35]);//校验位合�????????
	uint8_t rxData[32]={0};
	uint8_t CRCdata[35]={0};
	for(int j=0;j<35;j++){
		CRCdata[j]=g_uart_rx_buffer[j];
	}
	if (g_uart_frame_received) {
		g_uart_frame_received = 0;
		//		for (int i = 0; i < FRAME_RECEIVE_LENGTH; i++) {
		//			printf("Byte %d: %02X\n",i,g_uart_rx_buffer[i]);//打印接收的数�???????????
		//		}
		if( calculateCRC(CRCdata, sizeof(CRCdata)== receivedCRC)){
			for(int i=0;i<32;i++){
				rxData[i]=g_uart_rx_buffer[i+3];
			}
			Get_AC_Param(rxData);
			// 解析接收到的16字节数据
		}
		else{
			//			   printf("[IM1281B] U_in=%f\n", AC_IN.U);
			//			   printf("[IM1281B] I_in=%f\n", AC_IN.I);
			//			   printf("[IM1281B] P_in=%f\n", AC_IN.P);
			//			   printf("[IM1281B] PF_in=%f\n", AC_IN.PF);
			//			   printf("[IM1281B] F_in=%f\n", AC_IN.F);
			printf("[IM1281B] CRC ERROR\n");
		}
	}
	// 清空缓冲�??????????
	memset(g_uart_rx_buffer, 0, FRAME_RECEIVE_LENGTH);
}

void ModBus_Test(){
	ModBus_Send();
	ModBus_Receive();
}

